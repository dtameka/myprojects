-- процедура добавления P2P проверки
DROP PROCEDURE IF EXISTS prс_add_p2p;
CREATE OR REPLACE PROCEDURE prс_add_p2p(IN peer_checked varchar,
										IN peer_checking varchar,
										IN task_name varchar,
										IN status check_status DEFAULT 'Start',
										IN check_time time DEFAULT CURRENT_TIME(1))
LANGUAGE plpgsql as
$$
DECLARE
    check_id uuid := NULL;
BEGIN
	-- Если задан статус "Start", добавить запись в таблицу Checks
    IF status = 'Start' THEN
		-- Проверить что нет уже начатых проверок в связке проверяющий, проверяемый и проект
		IF NOT EXISTS(SELECT checks.id FROM p2p p1
						LEFT JOIN p2p p2 on p1."check" = p2."check" AND p2.state != 'Start'
						INNER JOIN checks on p1."check" = checks.id
						WHERE p1.state = 'Start' AND p2.id IS NULL AND checks.task = task_name
	  						  AND checks.peer = peer_checked AND p1."checkingPeer" = peer_checking)
        THEN -- нет незавершенной P2P проверки, относящейся к конкретному заданию, пиру и проверяющему
            INSERT INTO checks(peer, task, "date") VALUES (peer_checked, task_name, now()::date) RETURNING ID INTO check_id;
            INSERT INTO p2p("check", "checkingPeer", state, "time") VALUES (check_id, peer_checking, status, check_time::time);
        ELSE
            raise notice 'insert error: check_ID: % checkingPeer: % state: % checkTime: %', check_id, peer_checking, status, check_time::time;
        END IF;
    ELSE -- иначе указать проверку с незавершенным P2P этапом
        -- Находим id последней не завершенной проверки
        check_id := (SELECT checks.id FROM p2p p1
					 LEFT JOIN p2p p2 on p1."check" = p2."check" AND p2.state != 'Start'
					 INNER JOIN checks on p1."check" = checks.id
					 WHERE p1.state = 'Start' AND p2.id IS NULL AND checks.task = task_name AND checks.peer = peer_checked
					 	   AND p1."checkingPeer" = peer_checking AND p1.time <= check_time::time ORDER BY checks."date" DESC LIMIT 1);
        IF check_id IS NOT NULL THEN -- Если нашли, то добавляем запись в таблицу p2p с данным check_id
            INSERT INTO p2p("check", "checkingPeer", state, "time") VALUES (check_id, peer_checking, status, check_time::time);
        ELSE
            raise notice 'insert error: check_ID: % checkingPeer: % state: % checkTime: %', check_id, peer_checking, status, check_time::time;
        END IF;
    END IF;
END;
$$;

-- call prс_add_p2p('mayflower', 'wanayn', 'C2_SimpleBashUtils')
-- call prс_add_p2p('mayflower', 'wanayn', 'C2_SimpleBashUtils', 'Failure', (CURRENT_TIME(1) + '10 minute'::interval)::time)
-- call prс_add_p2p('mayflower', 'primat', 'C3_Stringplus')
-- call prс_add_p2p('mayflower', 'primat', 'C3_Stringplus', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time)

-- процедура добавления проверки Verter'ом
DROP PROCEDURE IF EXISTS prс_add_verter;
CREATE OR REPLACE PROCEDURE prс_add_verter(IN peer_checked varchar,
										IN task_name varchar,
										IN status check_status DEFAULT 'Start',
										IN check_time time DEFAULT CURRENT_TIME(1))
LANGUAGE plpgsql AS
$$
DECLARE
    check_id uuid := NULL;
BEGIN
	-- Если задан статус "Start"
    IF status = 'Start' THEN
		-- ищем успешно завершенную проверку p2p без начатой проверки verter
		check_id := (SELECT checks.id FROM p2p
					 LEFT JOIN verter ON verter."check" = p2p."check"
					 INNER JOIN checks on p2p."check" = checks.id
					 WHERE p2p.state = 'Success' AND verter.id IS NULL
					 	   AND checks.task = task_name AND checks.peer = peer_checked
					 ORDER BY checks."date" DESC LIMIT 1);
        IF check_id IS NOT NULL THEN -- Если нашли, то добавляем запись в таблицу с данным check_id
            INSERT INTO verter("check", state, "time") VALUES (check_id, status, check_time::time);
        ELSE
            raise notice 'insert error: check_ID: % state: % checkTime: %', check_id, status, check_time::time;
        END IF;
    ELSE -- иначе указать проверку с незавершенным этапом
        -- Находим id, последней не завершенной проверки
        check_id := (SELECT checks.id FROM verter v1
					 LEFT JOIN verter v2 ON v1."check" = v2."check" AND v2.state != 'Start'
					 INNER JOIN p2p on p2p."check" = v1."check"
					 INNER JOIN checks on p2p."check" = checks.id
					 WHERE p2p.state = 'Success' AND v2.id IS NULL
					 	   AND checks.task = task_name AND checks.peer = peer_checked
					 ORDER BY checks."date" DESC LIMIT 1);
        IF check_id IS NOT NULL THEN -- Если нашли, то добавляем запись в таблицу с данным check_id
            INSERT INTO verter("check", state, "time") VALUES (check_id, status, check_time::time);
        ELSE
            raise notice 'insert error: check_ID: % state: % checkTime: %', check_id, status, check_time::time;
        END IF;
    END IF;
END;
$$;

-- call prс_add_verter('mayflower', 'C2_SimpleBashUtils')
-- call prс_add_verter('mayflower', 'C2_SimpleBashUtils', 'Success')


-- Триггер: после добавления записи со статутом "Start" в таблицу P2P, изменить соответствующую запись в таблице TransferredPoints
DROP TRIGGER IF EXISTS trg_P2P_transferring_PRP_after_insert ON p2p;
DROP FUNCTION IF EXISTS fnc_trg_P2P_transferring_PRP_after_insert() cascade;

CREATE OR REPLACE FUNCTION fnc_trg_p2p_transferring_PRP_after_insert() RETURNS trigger AS $$
DECLARE
    checked_name text;
	pair_id uuid DEFAULT NULL;
BEGIN
    -- найти проверяемого(checked) через таблицу p2p
    checked_name := (SELECT peer FROM checks WHERE id = NEW.check);
	-- проверка была ли раньше запись о этой паре проверяющий + проверяемый
	pair_id := (SELECT id FROM transferredpoints WHERE "checkingPeer" = NEW."checkingPeer" AND "checkedPeer" = checked_name);
    IF NEW.state = 'Start'
	THEN
        IF pair_id IS NOT NULL
		THEN
            -- прибавляем prp к имеющийся записи
            UPDATE transferredpoints SET "pointsAmount" = "pointsAmount" + 1 WHERE id = pair_id;
        ELSE
            -- если не было записи с такими пирами в transferredpoints - добавляем новую запись в таблицу
            INSERT INTO transferredpoints("checkingPeer", "checkedPeer", "pointsAmount") VALUES (NEW."checkingPeer", checked_name, 1);
        END IF;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_P2P_transferring_PRP_after_insert AFTER INSERT ON p2p
FOR EACH ROW EXECUTE PROCEDURE fnc_trg_p2p_transferring_PRP_after_insert();

-- Триггер: перед добавлением записи в таблицу XP, проверить корректность добавляемой записи
DROP TRIGGER IF EXISTS trg_XP_check_on_insert ON xp;
DROP FUNCTION IF EXISTS fnc_trg_XP_check_on_insert() cascade;

CREATE OR REPLACE FUNCTION fnc_trg_XP_check_on_insert() RETURNS trigger AS $$
BEGIN
    -- Количество XP не превышает максимальное доступное для проверяемой задачи
    IF NEW.xpAmount <= (SELECT "maxXP" FROM checks
                          INNER JOIN tasks on tasks.title = checks.task AND checks.id = NEW."check")
    -- Поле Check ссылается на успешную проверку
    AND EXISTS(SELECT p2p."check" FROM p2p
               LEFT JOIN verter on p2p."check" = verter."check"
               WHERE p2p.state = 'Success' AND (verter.state = 'Success' OR verter.state IS NULL)
               AND p2p."check" = NEW."check")
    THEN
        RETURN NEW;
    ELSE
        raise exception 'insert error ID: % check: % xpAmount: %', NEW.id, NEW."check", NEW."xpAmount";
        RETURN NULL;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_XP_check_on_insert BEFORE INSERT ON xp
FOR EACH ROW EXECUTE PROCEDURE fnc_trg_XP_check_on_insert();
