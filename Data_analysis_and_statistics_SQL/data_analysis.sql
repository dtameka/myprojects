-- 3.1
CREATE OR REPLACE FUNCTION fnc_readable_transferredpoints() RETURNS TABLE ("Peer1" varchar, "Peer2" varchar, "PointsAmount" integer)
AS $$
BEGIN
	-- Если есть пиры, которых проверял человек, но они его не проверяли, запускается выборка таких людей
	CREATE OR REPLACE TEMP VIEW tmp_uuid AS
	SELECT DISTINCT t1.id, t1."checkingPeer", t1."checkedPeer", t1."pointsAmount"
	FROM transferredpoints AS t1
	LEFT JOIN transferredpoints AS t2 ON t2."checkedPeer" = t1."checkingPeer"
	WHERE t2."checkingPeer" IS NULL;

	-- Находим нужные пары пиров, которым нужно изменить pointsAmount на отрицательное и добавляем в tmp_negate
	CREATE OR REPLACE TEMP VIEW tmp_negate AS
	SELECT t1.id, t1."checkingPeer", t1."checkedPeer", t1."pointsAmount" FROM transferredpoints AS t1
	LEFT JOIN tmp_uuid ON tmp_uuid.id = t1.id
	WHERE tmp_uuid.id IS NULL;

	-- находим нужного пира и добавляем изменение в представление tmp_final
	CREATE OR REPLACE TEMP VIEW tmp_final AS
	SELECT t2.id, t2."checkingPeer", t2."checkedPeer", t2."pointsAmount" * (-1) AS "pointsAmount"
	FROM tmp_negate AS t1
	JOIN tmp_negate AS t2 ON t2."checkedPeer" = t1."checkingPeer" AND t2."checkingPeer" = t1."checkedPeer"
	WHERE t2."pointsAmount" < t1."pointsAmount";

	-- Составляем получившийся список
	-- Заменяем нашим готовым списком нужные позиции
	RETURN QUERY
	SELECT "checkingPeer", "checkedPeer", "pointsAmount" FROM tmp_final
	UNION
	(
	-- Убираем позиции, которые мы будем заменять, чтобы избежать копирования с положительными строками
	(
	SELECT DISTINCT t2."checkedPeer", t2."checkingPeer", COALESCE(0, 0, 0) AS "pointsAmount"
	FROM transferredpoints AS t1
	FULL JOIN transferredpoints AS t2 ON t2."checkedPeer" = t1."checkingPeer"
	WHERE t1."checkingPeer" IS NULL
	)
	UNION
	(
	SELECT t1."checkingPeer", t1."checkedPeer", t1."pointsAmount" FROM transferredpoints AS t1
	LEFT JOIN tmp_final ON tmp_final.id = t1.id
	WHERE tmp_final.id IS NULL
	)
	);
END;
$$ LANGUAGE plpgsql;

-- TEST 3.1
-- SELECT * FROM transferredpoints;
-- SELECT * FROM fnc_readable_transferredpoints();
-- inester проверил gasveni 0 раз, после проверки появится -1, потому что это меньше, чем получил с него gasveni
-- call prс_add_p2p('gasveni', 'inester', 'D01_Linux');
-- call prс_add_p2p('gasveni', 'inester', 'D01_Linux', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time);

-- 3.2
CREATE OR REPLACE FUNCTION fnc_peer_xp_for_tasks() RETURNS TABLE ("Peer" varchar, "Task" varchar, "XP" integer)
AS $$
SELECT t2.peer AS "Peer", t2.task AS "Task", xp.xpamount AS "XP"
FROM (SELECT "check", "checkingPeer", "state" FROM p2p WHERE "state" = 'Success') AS t1
JOIN checks AS t2 ON t2.id = t1."check"
JOIN xp ON xp."check" = t1."check";
$$ LANGUAGE SQL;

-- TEST 3.2
-- SELECT * FROM fnc_peer_xp_for_tasks()

-- 3.3
CREATE OR REPLACE FUNCTION fnc_nonstop_peers(IN pdate date) RETURNS TABLE ("Peer" varchar)
AS $$
SELECT t1.peer AS "Peer"
FROM (SELECT peer, COUNT(peer) FROM timetracking WHERE "date" = pdate GROUP BY peer) AS t1
WHERE t1."count" = 2
$$ LANGUAGE SQL;

-- TEST 3.3
-- SELECT * FROM fnc_nonstop_peers('2022-12-05')

-- 3.4
CREATE OR REPLACE PROCEDURE prc_view_success_and_unsuccess_checks(IN ref refcursor)
AS $$
DECLARE
	Success integer := (SELECT COUNT(state) AS "SuccessfulChecks" FROM p2p WHERE state = 'Success');
	Failure integer := (SELECT COUNT(state) AS "SuccessfulChecks" FROM p2p WHERE state = 'Failure');
BEGIN
	OPEN ref FOR
		SELECT round(Success * 100 / (Success + Failure)::numeric) AS "SuccessfulChecks",
		round(Failure * 100 / (Success + Failure)::numeric) AS "UnsuccessfulChecks";
END;
$$ LANGUAGE plpgsql;


-- TEST 3.4
-- BEGIN;
--	CALL prc_view_success_and_unsuccess_checks('name');
--	FETCH ALL IN "name";
-- ROLLBACK;

-- 3.5
CREATE OR REPLACE FUNCTION fnc_get_peer_points_change(IN peername varchar)
RETURNS integer
AS $$
BEGIN
-- Отнимает от полученых поинтов потраченые, получая итоговое кол-во поинтов.
-- кол-во может быть как отрицательным так и положительным.
RETURN
(
-- Полученные поинты
	(SELECT COALESCE(
		(SELECT(SUM("pointsAmount"))
         FROM transferredpoints
         WHERE "checkingPeer" = peername
         GROUP BY "checkingPeer"), 0))
     -
-- Потраченные поинты
    (SELECT COALESCE(
		(SELECT(SUM("pointsAmount")) 
         FROM transferredpoints
         WHERE "checkedPeer" = peername
         GROUP BY "checkedPeer"), 0))
);
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE PROCEDURE prc_peer_points_changes(IN ref refcursor)
AS $$
BEGIN
OPEN ref FOR
SELECT nickname AS "Peer", fnc_get_peer_points_change(nickname) AS "PointsChange"
FROM (
	SELECT "checkingPeer" AS nickname
    FROM transferredpoints
    UNION DISTINCT
    SELECT "checkedPeer" AS nickname
    FROM transferredpoints 
	) AS nicknames
ORDER BY "PointsChange" DESC;
END; 
$$ LANGUAGE plpgsql;

-- TEST 3.5
-- BEGIN;
-- 	CALL prc_peer_points_changes('name');
-- 	FETCH ALL IN "name";
-- ROLLBACK;

-- 3.6
CREATE OR REPLACE PROCEDURE prc_peer_points_changes2(IN ref refcursor)
AS $$
BEGIN
OPEN ref FOR
SELECT nickname AS "Peer", fnc_get_peer_points_change(nickname) AS "PointsChange"
FROM (
	SELECT "Peer1" AS nickname
    FROM fnc_readable_transferredpoints()
    UNION DISTINCT
    SELECT "Peer2" AS nickname
    FROM fnc_readable_transferredpoints()
    ) AS nicknames
    ORDER BY "PointsChange" DESC;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.6
--BEGIN;
--  CALL prc_peer_points_changes2('name');
--  FETCH ALL IN "name";
--ROLLBACK;

-- 3.7
--  Определяет самое часто проверяемое задание за определённый день
CREATE OR REPLACE FUNCTION fnc_get_max_checks_task_for_days(IN pdate date)
RETURNS integer AS $$
BEGIN
RETURN (
	WITH checksfordays AS (
	SELECT ch.date, ch.task, COUNT(*)
	FROM checks AS ch
	GROUP BY ch.date, ch.task
	ORDER BY ch.date
	)
	SELECT MAX(cfd.count)
	FROM checksfordays AS cfd
	WHERE cfd.date = pdate);
END;
$$ LANGUAGE plpgsql;

--  Определяет самое часто проверяемое задание за каждый день
CREATE OR REPLACE PROCEDURE prc_most_checked_task_for_day(IN ref refcursor)
AS $$
BEGIN
OPEN ref FOR
    SELECT to_char(date, 'DD.MM.YYYY') AS day, tmp1.task
    FROM(
		SELECT ch.date, ch.task, COUNT(*)
        FROM checks AS ch
        GROUP BY ch.date, ch.task
		ORDER BY ch.date
		) AS tmp1
    WHERE "count" = fnc_get_max_checks_task_for_days(date);
END;
$$ LANGUAGE plpgsql;

-- TEST 3.7
-- BEGIN;
--	CALL prc_most_checked_task_for_day('cursor_name');
--  FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.8
CREATE OR REPLACE PROCEDURE prc_duration_last_check(IN ref refcursor)
AS $$
DECLARE
-- check последней завершённой проверки
need_check uuid :=
	(
    SELECT p1."check"
    FROM p2p p1
    JOIN p2p p2 ON p2."check" = p1."check" and p2."state" = 'Start'
    WHERE p1.state != 'Start'
    ORDER BY p1.time desc
    LIMIT 1
	);
	
-- Время начала проверки
starttime time := 
	(
	SELECT time FROM p2p
    WHERE "check" = need_check AND "state" = 'Start'
    ORDER BY time DESC
    LIMIT 1
    );
-- Время конца проверки
endtime time :=
	(
	SELECT time 
	FROM p2p
    WHERE "check" = need_check AND "state" != 'Start'
    ORDER BY time DESC
    LIMIT 1
	);
BEGIN
    OPEN ref FOR
    SELECT endtime - starttime AS check_duration;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.8
-- BEGIN;
--  CALL prc_duration_last_check('cursor_name');
--  FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.9
CREATE OR REPLACE PROCEDURE prc_peers_completed_all_unit(IN ref refcursor, IN blockname varchar) AS $$
BEGIN
OPEN ref FOR
with completed_blocks AS (
SELECT ch.peer, case -- получить кол-во выполненых задач пира
WHEN COUNT(
	DISTINCT(SELECT ch.task
    FROM p2p p2
    JOIN checks AS ch ON ch.id = p2."check"
    WHERE p2."state" = 'Success' AND p2."check" = p1."check"))
    = -- и сравнить с кол-вом всех задач в блоке
    (SELECT COUNT(*)
    FROM tasks
    WHERE tasks.title LIKE (blockname||'%'))
    THEN ch.date
    END AS day
FROM p2p p1
JOIN checks AS ch ON ch.id = p1."check"
WHERE ch.task LIKE (blockname||'%') AND state='Success'
GROUP BY ch.peer, ch.date)
SELECT cb.peer, cb.day
FROM completed_blocks AS cb
WHERE cb.day IS NOT null
ORDER BY day DESC;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.9
-- На данный момент никто не закончил хотябы 1 ветку, давайте это исправим на примере DevOps ветки

-- call prс_add_p2p('gasveni', 'inester', 'D02_Linux_Nexwork');
-- call prс_add_p2p('gasveni', 'inester', 'D02_Linux_Nexwork', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time);
-- call prс_add_p2p('gasveni', 'inester', 'D03_LinuxMonitoring_v1');
-- call prс_add_p2p('gasveni', 'inester', 'D03_LinuxMonitoring_v1', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time);
-- call prс_add_p2p('gasveni', 'inester', 'D04_LinuxMonitoring_v2');
-- call prс_add_p2p('gasveni', 'inester', 'D04_LinuxMonitoring_v2', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time);
-- call prс_add_p2p('gasveni', 'inester', 'D05_SimpleDocker');
-- call prс_add_p2p('gasveni', 'inester', 'D05_SimpleDocker', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time);
-- call prс_add_p2p('gasveni', 'inester', 'D06_CICD');
-- call prс_add_p2p('gasveni', 'inester', 'D06_CICD', 'Success', (CURRENT_TIME(1) + '10 minute'::interval)::time);

-- BEGIN;
--  CALL prc_peers_completed_all_unit('cursor_name', 'D0');
--  FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.10
CREATE OR REPLACE PROCEDURE prc_get_recommended_reviewers(IN ref refcursor) AS $$
BEGIN
    OPEN ref FOR
    WITH friendRecommendedPeer AS (
		SELECT p.nickname as peer,
			   f.peer2 as friend,
			   r.recommendedpeer as recommendedpeer
			FROM peers p
			INNER JOIN friends f ON p.nickname = f.peer1
			INNER JOIN recommendations r ON f.peer2 = r.peer AND p.nickname != r.recommendedpeer
		ORDER BY peer, friend
	), countRecommendations AS (
    	SELECT peer,
			   recommendedpeer,
			   count(recommendedpeer) AS count_recommendations
			FROM friendRecommendedPeer
		GROUP BY peer, recommendedpeer
		ORDER BY peer, recommendedpeer
	), howOften AS (
    	SELECT peer,
			   recommendedpeer,
			   count_recommendations,
			   ROW_NUMBER() OVER (PARTITION BY peer ORDER BY count_recommendations DESC) AS num_of_row_for_each_peer
			FROM countRecommendations
    )

    SELECT peer, recommendedpeer FROM howOften WHERE num_of_row_for_each_peer = 1;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.10
--BEGIN;
--  CALL prc_get_recommended_reviewers('cur_part10');
--  FETCH ALL FROM "cur_part10";
--END;

-- 3.11
CREATE OR REPLACE FUNCTION fnc_peers_who_start_block("block" varchar) 
    RETURNS table("Peer" varchar) AS $$ 
    SELECT DISTINCT ch.peer
    FROM Checks AS ch
    WHERE ch.task LIKE "block" || '%'
$$ LANGUAGE sql;

CREATE OR REPLACE PROCEDURE prc_peer_progress_in_blocks(IN ref refcursor, IN "block1" varchar, IN "block2" varchar) 
AS $$
DECLARE
	StartedBlock1 integer := (SELECT COUNT(*) FROM fnc_peers_who_start_block("block1"));

	StartedBlock2 integer := (SELECT COUNT(*) FROM fnc_peers_who_start_block("block2"));

 	StartedBothBlocks integer := (SELECT COUNT(*)
					FROM
					(
                                  	SELECT "Peer"  FROM fnc_peers_who_start_block("block1")
                                        INTERSECT
                                        SELECT "Peer"  FROM fnc_peers_who_start_block("block2")
                                        ) AS res);

 	DidntStartAnyBlock integer := (SELECT count(*)
					FROM
					(
                                        SELECT nickname FROM peers
                                        EXCEPT
                                        (
					SELECT "Peer" FROM fnc_peers_who_start_block("block1")
                                        UNION
                                        SELECT "Peer" FROM fnc_peers_who_start_block("block2")
					)
                   			) AS res1);

        AllPeers integer := (SELECT count(*) FROM peers);

BEGIN
OPEN ref FOR
	SELECT round(StartedBlock1 * 100 / AllPeers::numeric) AS "StartedBlock1", 
           round(StartedBlock2 * 100 / AllPeers::numeric) AS "StartedBlock2", 
           round(StartedBothBlocks * 100 / AllPeers::numeric) AS "StartedBothBlocks", 
           round(DidntStartAnyBlock * 100 / AllPeers::numeric) AS "DidntStartAnyBlock";
END;
$$ LANGUAGE plpgsql;

-- TEST 3.11
--BEGIN;
--	CALL prc_peer_progress_in_blocks('cursor_name', 'CPP', 'C');
--  FETCH ALL IN "cursor_name";
--ROLLBACK;

-- 3.12
CREATE OR REPLACE PROCEDURE prс_peer_who_have_more_friends(IN ref refcursor, IN flimit integer) 
AS $$
BEGIN
OPEN ref FOR
        SELECT peer1 AS "Peer", COUNT(*)
		FROM (
              SELECT peer1 FROM friends
              UNION ALL
              SELECT peer2 FROM friends
			 ) AS res
        GROUP BY peer1
        ORDER BY "count" DESC
        LIMIT flimit;
    END; 
$$ LANGUAGE plpgsql;

-- TEST 3.12
-- BEGIN;
--  CALL prс_peer_who_have_more_friends('cursor_name', 2);
--  FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.13
CREATE OR REPLACE FUNCTION fnc_checks_on_birthday(pstate check_status) 
RETURNS integer 
AS $$
SELECT COUNT(*)
FROM p2p
JOIN checks ON checks.id = p2p.check
LEFT JOIN peers ON peers.nickname = checks.peer 
WHERE substring(checks.date::text, 6) = substring(peers.birthday::text, 6) AND p2p.state = pstate
$$ LANGUAGE sql;

CREATE OR REPLACE PROCEDURE prс_percent_checks_on_birthday(IN ref refcursor) 
AS $$
DECLARE 
	Success integer := (SELECT * FROM fnc_checks_on_birthday('Success'));
	Failure integer := (SELECT * FROM fnc_checks_on_birthday('Failure'));
BEGIN
OPEN ref FOR
	SELECT  round(NULLIF(Success,0) * 100 / (NULLIF(Success,0) + Failure)::numeric) AS "SuccessfulChecks", 
            round(NULLIF(Failure,0) * 100 / (Success + NULLIF(Failure,0))::numeric) AS "UnsuccessfulChecks";
END;
$$ LANGUAGE plpgsql;

-- TEST 3.13
-- Поменяем уже существующим проверкам даты, будто они происходили на день рождения
-- сделаем 1 проверку Success, а другую Failure, всё в транзакции.

-- BEGIN;
-- UPDATE checks SET date = '2023-07-05' WHERE id = 'f5e7fc36-8e8f-11ed-9a42-0242ac110002';
-- UPDATE checks SET date = '2023-07-20' WHERE id = '2119a4c2-8e90-11ed-9a42-0242ac110002';
-- CALL prс_percent_checks_on_birthday('cursor_name');
-- FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.14
CREATE OR REPLACE PROCEDURE prc_sum_xp(IN ref refcursor)
AS $$
BEGIN
OPEN ref FOR
SELECT peer AS "Peer", SUM(xp) AS "XP"
    FROM(
		SELECT peers.nickname AS peer, tasks.title AS task, MAX(xp.xpamount) AS xp
        FROM xp 
		JOIN checks ON checks.id = xp.check
		JOIN tasks ON tasks.title = checks.task
        JOIN peers ON peers.nickname = checks.peer
        GROUP by 1, 2
		) AS tmp
    GROUP BY peer
	ORDER BY 2 DESC;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.14
-- BEGIN;
--   CALL prc_sum_xp('cursor_name');
--   FETCH ALL FROM "cursor_name";
-- ROLLBACK;

-- 3.15
CREATE OR REPLACE FUNCTION fnc_peer_complete_task(ppeer varchar, ptask varchar) 
RETURNS bool 
AS $$ 
SELECT EXISTS (
	SELECT checks.peer 
	FROM p2p
	JOIN checks ON checks.id = p2p.check
	WHERE checks.peer = ppeer AND checks.task = ptask AND p2p.state = 'Success'
)
$$ LANGUAGE SQL;

CREATE OR REPLACE PROCEDURE prc_find_peers_part15(IN ref refcursor, IN first_task varchar, IN second_task varchar, IN third_task varchar)
AS $$
BEGIN
OPEN ref FOR
	SELECT DISTINCT peers.nickname 
	FROM peers
    JOIN checks ON checks.peer = peers.nickname
    WHERE fnc_peer_complete_task(peers.nickname, first_task) 
    AND fnc_peer_complete_task(peers.nickname, second_task)
    AND NOT fnc_peer_complete_task(peers.nickname, third_task);
END; 
$$ LANGUAGE plpgsql;

-- TEST 3.15
-- BEGIN;
--  CALL prc_find_peers_part15('cursor_name', 'C2_SimpleBashUtils', 'C3_Stringplus', 'CPP3_SmartCalc_v2');
--  FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.16
CREATE OR REPLACE FUNCTION count_parent_tasks(ftask varchar) 
RETURNS integer AS $$ 
    WITH RECURSIVE counter AS (
        SELECT title, "parentTask", 0 as tier
        FROM tasks 
        WHERE title = ftask
        UNION ALL 
        SELECT tasks.title, tasks."parentTask", counter.tier + 1
        FROM tasks
        JOIN counter ON tasks.title = counter."parentTask"
    )
    SELECT MAX(tier) FROM counter
$$ LANGUAGE SQL;

CREATE OR REPLACE PROCEDURE prc_count_parents_for_tasks(IN ref refcursor) AS $$ 
BEGIN
OPEN ref FOR
	SELECT tasks.title, count_parent_tasks(tasks.title) 
	FROM tasks;
END;
$$ LANGUAGE plpgsql;


-- TEST 3.16
-- BEGIN;
--   CALL prc_count_parents_for_tasks('cursor_name');
--   FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.17
CREATE OR REPLACE PROCEDURE prc_lucky_days(IN ref refcursor, IN N integer) 
AS $$ 
BEGIN
OPEN ref FOR
WITH tmp AS (
	SELECT id, task, state, prev, date,
	CASE WHEN "state" = 'Success' AND ("state" = prev OR prev IS NULL)
		THEN 1
		ELSE 0
		END, "maxXP", "xpamount"
    FROM (
         SELECT checks.id, checks.date, checks.peer, checks.task, p2p.state, LAG(p2p.state) OVER(PARTITION BY checks.date ORDER BY checks.date) as prev,
		 "maxXP", "xpamount"
         FROM p2p
		 JOIN checks ON checks.id = p2p.check
		 LEFT JOIN xp ON xp.check = checks.id
		 LEFT JOIN tasks ON tasks.title = checks.task
		 WHERE state = 'Success' OR state = 'Failure'
         ) AS res
	ORDER BY date
)
SELECT t1."date"
FROM(
	SELECT date, COUNT(date) AS "count"
	FROM tmp
	WHERE "state" = 'Success' AND ("state" = prev OR prev IS NULL) AND tmp.xpamount > tmp."maxXP" * 0.8
	GROUP BY date
) AS t1
GROUP BY t1.date
HAVING MAX(count) >= N;
END; 
$$ LANGUAGE plpgsql;


--TEST 3.17
-- Нужно дополнительно добавить xp нашим проверкам, что мы делали выше
-- BEGIN;
-- INSERT INTO xp("check", xpAmount)
-- VALUES ((SELECT id FROM checks WHERE task = 'D01_Linux'), 300),
-- ((SELECT id FROM checks WHERE task = 'D02_Linux_Nexwork'), 350),
-- ((SELECT id FROM checks WHERE task = 'D03_LinuxMonitoring_v1'), 350),
-- ((SELECT id FROM checks WHERE task = 'D04_LinuxMonitoring_v2'), 350),
-- ((SELECT id FROM checks WHERE task = 'D05_SimpleDocker'), 300),
-- ((SELECT id FROM checks WHERE task = 'D06_CICD'), 300);
--     CALL prc_lucky_days('cursor_name', 1);
--     FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 3.18
CREATE OR REPLACE PROCEDURE prс_highest_comp_tasks(IN ref refcursor) 
AS $$
BEGIN
OPEN ref FOR
WITH Success_tasks AS (
	SELECT checks.peer, COUNT(*)
	FROM p2p
	JOIN checks ON checks.id = p2p.check
    WHERE p2p.state = 'Success'
    GROUP BY peer
	ORDER BY count DESC
	LIMIT 1
)
SELECT st.peer AS "Peer", st.count AS "Tasks" 
FROM Success_tasks AS st;
END;
$$ LANGUAGE plpgsql;

--TEST 3.18
-- BEGIN;
--     CALL prс_highest_comp_tasks('cursor_name');
--     FETCH ALL IN "cursor_name";
-- ROLLBACK;

-- 19) Определить пира с наибольшим количеством XP
CREATE OR REPLACE PROCEDURE prc_get_max_xp(IN ref refcursor) AS $$
BEGIN
	OPEN ref FOR
	WITH maxXP AS (
		SELECT peer, SUM(xpamount) FROM xp INNER JOIN checks ON checks.id = xp."check" GROUP BY peer
	)
	SELECT maxXP.peer AS "Peer",
		   maxXP.sum AS "XP"
		FROM maxXP
		WHERE sum = (SELECT max(maxXP.sum) FROM maxXP);
END;
$$ LANGUAGE plpgsql;

-- TEST 3.19
-- BEGIN;
-- 	CALL prc_get_max_xp('cur_part19');
--     FETCH ALL IN cur_part19;
-- ROLLBACK;

-- 20) Определить пира, который провел сегодня в кампусе больше всего времени
CREATE OR REPLACE PROCEDURE prc_get_max_time_on_campus_today(IN ref refcursor) AS $$
BEGIN
	OPEN ref FOR
	WITH presenceTime AS (
		SELECT tt.peer,
	   		   SUM(tt.time - (SELECT tt2.time FROM timetracking AS tt2
							  	WHERE tt2.state = '1' AND tt2.peer = tt.peer AND tt2.date = tt.date AND tt2.time <= tt.time
							  	ORDER BY tt2.time DESC LIMIT 1)) AS presence_time
			FROM timetracking AS tt WHERE tt.state = '2' AND tt.date = now()::date
			GROUP BY tt.peer
			ORDER BY presence_time DESC
	)
	SELECT peer AS "Peer" FROM presenceTime WHERE presence_time = (SELECT max(presence_time) FROM presenceTime);
END;
$$ LANGUAGE plpgsql;

-- TEST 3.20
-- BEGIN;
-- 	INSERT INTO timetracking(peer, date, "time", state)
-- 	VALUES ('helixo', CURRENT_DATE, '08:02', '1'), 
-- 	('quintent', CURRENT_DATE, '08:02', '1'),
-- 	('helixo', CURRENT_DATE, '09:00', '2'), 
-- 	('quintent', CURRENT_DATE, '10:00', '2');

-- 	CALL prc_get_max_time_on_campus_today('cur_part20');
--     FETCH ALL IN cur_part20;
-- ROLLBACK;

-- 21) Определить пиров, приходивших раньше заданного времени не менее N раз за всё время
CREATE OR REPLACE PROCEDURE prc_get_came_ahead_time(IN ref refcursor, search_time time, n integer) AS $$
BEGIN
    OPEN ref FOR
	WITH cameAheadTime AS (
		SELECT peer,
			   "date",
			   min("time")
			FROM timetracking
			WHERE "state" = '1' AND "time" < search_time
			GROUP BY peer, date
			ORDER BY date
	)
	SELECT peer FROM cameAheadTime GROUP BY peer HAVING count("date") >= n;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.21
-- BEGIN;
--     CALL prc_get_came_ahead_time('cur_part21', '12:00:00', 3);
--     FETCH ALL IN "cur_part21";
-- ROLLBACK;

-- 22) Определить пиров, выходивших за последние N дней из кампуса больше M раз
CREATE OR REPLACE PROCEDURE prc_get_left_campus_more(IN ref refcursor, n integer, m integer) AS $$
BEGIN
    OPEN ref FOR
	WITH leftCampus AS (
		SELECT peer,
			   "date",
			   (count(*) - 1) AS count
			FROM timetracking
			WHERE "state" = '1' AND "date" > (now()::date - n)
			GROUP BY peer, date
			ORDER BY date
	)
	SELECT peer FROM leftCampus GROUP BY peer HAVING sum(count) > m;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.22
-- BEGIN;
--     CALL prc_get_left_campus_more('cur_part22', 55, 0);
--     FETCH ALL IN "cur_part22";
-- ROLLBACK;

-- 23) Определить пира, который пришел сегодня последним
CREATE OR REPLACE PROCEDURE prc_get_came_last_peer(IN ref refcursor) AS $$
BEGIN
    OPEN ref FOR
	WITH cameLast AS (
		SELECT peer,
			   "date",
			   min("time") AS first_entry
			FROM timetracking
			WHERE "state" = '1' AND "date" = now()::date
			GROUP BY peer, "date"
	)
    SELECT peer FROM cameLast ORDER BY first_entry DESC LIMIT 1;
END;
$$ LANGUAGE plpgsql;

-- TEST 3.23
--BEGIN;
--INSERT INTO timetracking(peer, date, "time", state)
--	VALUES ('helixo', CURRENT_DATE, '08:02', '1'), 
-- 	('quintent', CURRENT_DATE, '08:20', '1'),
-- 	('helixo', CURRENT_DATE, '09:00', '2'), 
-- 	('quintent', CURRENT_DATE, '10:00', '2');
--	CALL prc_get_came_last_peer('cur_part23');
--    FETCH ALL IN cur_part23;
--ROLLBACK;

-- 24) Определить пиров, которые выходили вчера из кампуса больше чем на N минут
CREATE OR REPLACE PROCEDURE prc_get_peers_left_campus_more(IN ref refcursor, N integer) AS $$
BEGIN
    OPEN ref FOR
	WITH leftTime AS (
		SELECT tt.peer,
			   (tt.time - (SELECT tt2.time FROM timetracking AS tt2
							  	WHERE tt2.state = '2' AND tt2.peer = tt.peer AND tt2.date = tt.date AND tt2.time <= tt.time
							  	ORDER BY tt2.time DESC LIMIT 1)) AS left_time
			FROM timetracking AS tt WHERE tt.state = '1' AND tt.date = now()::date - 1
			AND NOT time IN (SELECT min(time) FROM timetracking WHERE state = '1' AND peer = tt.peer AND date = tt.date)
			ORDER BY left_time DESC
	)
	
	SELECT peer AS "Peer" FROM leftTime GROUP BY peer HAVING sum(left_time) >= make_time(n / 60, n - n / 60 * 60, 0.0);
-- 	make_time ( hour int, min int, sec double precision ) → time
-- 	Create time from hour, minute and seconds fields
-- 	make_time(8, 15, 23.5) → 08:15:23.5
END;
$$ LANGUAGE plpgsql;

-- TEST 3.24
-- BEGIN;
-- INSERT INTO timetracking(peer, date, "time", state)
--  	VALUES ('helixo', CURRENT_DATE-1, '08:02', '1'), 
--  	('quintent', CURRENT_DATE-1, '08:02', '1'),
--  	('helixo', CURRENT_DATE-1, '08:10', '2'), 
--  	('quintent', CURRENT_DATE-1, '08:10', '2'),
-- 	('helixo', CURRENT_DATE-1, '08:20', '1'), 
--  	('quintent', CURRENT_DATE-1, '10:00', '1'),
--  	('helixo', CURRENT_DATE-1, '12:10', '2'), 
--  	('quintent', CURRENT_DATE-1, '12:10', '2');
	
--     CALL prc_get_peers_left_campus_more('cur_part24', 60);
--     FETCH ALL IN "cur_part24";
-- ROLLBACK;

-- 25) Определить для каждого месяца процент ранних входов
CREATE OR REPLACE PROCEDURE prc_visits_in_month_of_birth(IN ref refcursor) AS $$
BEGIN
	OPEN ref FOR
	WITH VisitsInMonthOfBirth AS (
		SELECT TO_CHAR(TO_DATE(month_num::text, 'MM'), 'Month') AS "Month",
			   (SELECT count(tt.peer)
				FROM (SELECT peer, date FROM timetracking
					  WHERE peer IN (SELECT nickname FROM peers WHERE EXTRACT(month FROM birthday) = month_num)
					  GROUP BY peer, date HAVING min("time") <= '24:00:00') AS tt) AS "AllVisits",
			   (SELECT count(tt.peer)
				FROM (SELECT peer, date FROM timetracking
					  WHERE peer IN (SELECT nickname FROM peers WHERE EXTRACT(month FROM birthday) = month_num)
					  GROUP BY peer, date HAVING min("time") <= '12:00:00') AS tt) AS "EarlyVisits"
		FROM generate_series(1, 12) AS month_num
	)

	SELECT "Month",
		   CASE WHEN "AllVisits" = 0 THEN 0
				ELSE ("EarlyVisits" * 100 / "AllVisits") 
		   END AS "EarlyEntries"
		FROM VisitsInMonthOfBirth
		ORDER BY to_date("Month", 'Mon');
END;
$$ LANGUAGE plpgsql;

-- TEST 3.25
-- BEGIN;
-- 	CALL prc_visits_in_month_of_birth('cur_part25');
--     FETCH ALL IN cur_part25;
-- ROLLBACK;
