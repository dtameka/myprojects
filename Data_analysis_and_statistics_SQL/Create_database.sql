-- Удаляем все таблицы и типы которые необходимо создать
-- drop table if exists peers cascade;
-- drop table if exists tasks cascade;
-- drop type if exists check_status cascade;
-- drop table if exists checks cascade;
-- drop table if exists p2p cascade;
-- drop trigger if exists trg_p2p_check_on_insert ON p2p;
-- drop function if exists fnc_trg_p2p_check_on_insert() cascade;
-- drop table if exists verter cascade;
-- drop trigger if exists trg_verter_check_on_insert ON p2p;
-- drop function if exists fnc_trg_verter_check_on_insert() cascade;
-- drop table if exists transferredpoints cascade;
-- drop table if exists friends cascade;
-- drop table if exists recommendations cascade;
-- drop table if exists xp cascade;
-- drop type if exists peer_visit_state cascade;
-- drop table if exists timetracking cascade;
-- drop trigger if exists trg_timetracking_check_on_insert ON timetracking;
-- drop function if exists fnc_trg_timetracking_check_on_insert() cascade;

-- Таблица Peers
create table if not exists peers (
	nickname varchar not null primary key,
	birthday date not null
);
COMMENT ON COLUMN peers.nickname IS 'Ник пира';
COMMENT ON COLUMN peers.birthday IS 'День рождения';

-- Таблица Tasks
create table if not exists tasks (
    title varchar not null primary key,
    "parentTask" varchar default null,
    constraint "fk_tasks_parentTask" foreign key ("parentTask") references tasks(title),
    "maxXP" integer not null
);
COMMENT ON COLUMN tasks.title IS 'Название задания';
COMMENT ON COLUMN tasks."parentTask" IS 'Название задания, являющегося условием входа';
COMMENT ON COLUMN tasks."maxXP" IS 'Максимальное количество XP';

-- Статус проверки
create type check_status as enum ('Start', 'Success', 'Failure');

-- UUID generator 
create extension if not exists "uuid-ossp";

-- Таблица Checks
create table if not exists checks (
    id uuid primary key default uuid_generate_v1(),
    peer varchar, constraint fk_checks_peer foreign key (peer) references peers(nickname),
    task varchar, constraint fk_checks_task foreign key (task) references tasks(title),
    "date" date
);
COMMENT ON COLUMN checks.peer IS 'Ник пира';
COMMENT ON COLUMN checks.task IS 'Название задания';
COMMENT ON COLUMN checks."date" IS 'Дата проверки';

-- Таблица P2P
create table if not exists p2p (
    id uuid primary key default uuid_generate_v1(),
    "check" uuid not null, constraint fk_p2p_check foreign key ("check") references checks(id),
    "checkingPeer" varchar not null,
	constraint fk_p2p_checkingPeer foreign key ("checkingPeer") references peers(nickname),
    state check_status,
    time time without time zone not null
);
COMMENT ON COLUMN p2p."check" IS 'ID проверки';
COMMENT ON COLUMN p2p."checkingPeer" IS 'Ник проверяющего пира';
COMMENT ON COLUMN p2p.state IS 'Статус P2P проверки';
COMMENT ON COLUMN p2p.time IS 'Время';

-- Триггер: перед добавлением записи в таблицу P2P, проверить что время завершение проверки не меньше времени начала
CREATE OR REPLACE FUNCTION fnc_trg_p2p_check_on_insert() RETURNS trigger AS $$
BEGIN
    IF NEW.state != 'Start' AND NEW.time >= (SELECT time FROM p2p WHERE "check" = NEW."check" AND state = 'Start') THEN
        RETURN NEW;
    ELSIF NEW.state = 'Start' THEN
       	RETURN NEW;
    ELSE
       	raise exception 'insert error check: % checkingPeer: % state: % time: %', NEW."check", NEW."checkingPeer", NEW.state, NEW.time;
       	RETURN NULL;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_p2p_check_on_insert BEFORE INSERT ON p2p
FOR EACH ROW EXECUTE PROCEDURE fnc_trg_p2p_check_on_insert();

-- Таблица Verter
create table if not exists verter (
    id uuid primary key default uuid_generate_v1(),
    "check" uuid not null, constraint fk_verter_check foreign key ("check") references checks(id),
    state check_status,
    time time without time zone not null
);
COMMENT ON COLUMN verter."check" IS 'ID проверки';
COMMENT ON COLUMN verter.state IS 'Статус проверки Verter''ом';
COMMENT ON COLUMN verter.time IS 'Время';

-- Триггер: перед добавлением записи в таблицу Verter, проверить что время завершение проверки не меньше времени начала и есть завершенная проверка p2p
CREATE OR REPLACE FUNCTION fnc_trg_verter_check_on_insert() RETURNS trigger AS $$
BEGIN
	
	IF NEW.state = 'Start' AND EXISTS(SELECT id FROM p2p WHERE state = 'Success' AND "check" = NEW."check") THEN
		RETURN NEW;
    ELSIF NEW.state != 'Start' AND NEW.time >= (SELECT time FROM verter WHERE "check" = NEW."check" AND state = 'Start') THEN
        RETURN NEW;
    ELSE
		raise exception 'insert error ID: % check: % checkingPeer: %', NEW.id, NEW."check", NEW."checkingPeer";
		RETURN NULL;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_verter_check_on_insert BEFORE INSERT ON verter
FOR EACH ROW EXECUTE PROCEDURE fnc_trg_verter_check_on_insert();

-- Таблица TransferredPoints
create table if not exists transferredpoints (
    id uuid primary key default uuid_generate_v1(),
    "checkingPeer" varchar not null,
	constraint fk_transferredpoints_checkingPeer foreign key ("checkingPeer") references peers(nickname),
    "checkedPeer" varchar not null,
	constraint fk_transferredpoints_checkedPeer foreign key ("checkedPeer") references peers(nickname),
	constraint unique_transferredpoints_checkingPeer_checkedPeer unique ("checkingPeer", "checkedPeer"),
    "pointsAmount" integer
);
COMMENT ON COLUMN transferredpoints."checkingPeer" IS 'Ник проверяющего пира';
COMMENT ON COLUMN transferredpoints."checkedPeer" IS 'Ник проверяемого пира';
COMMENT ON COLUMN transferredpoints."pointsAmount" IS 'Количество переданных пир поинтов за всё время (только от проверяемого к проверяющему)';

-- Таблица Friends
create table if not exists friends (
    id uuid primary key default uuid_generate_v1(),
    peer1 varchar not null, constraint fk_friends_peer1 foreign key (peer1) references peers(nickname),
    peer2 varchar not null, constraint fk_friends_peer2 foreign key (peer2) references peers(nickname)
);
COMMENT ON COLUMN friends.peer1 IS 'Ник первого пира';
COMMENT ON COLUMN friends.peer2 IS 'Ник второго пира';

-- Таблица Recommendations
create table if not exists recommendations (
	id uuid primary key default uuid_generate_v1(),
    peer varchar, constraint fk_recommendations_peer foreign key (peer) references peers(nickname),
    recommendedPeer varchar,
	constraint fk_recommendations_recommendedPeer foreign key (recommendedPeer) references peers(nickname)
);
COMMENT ON COLUMN recommendations.peer IS 'Ник пира';
COMMENT ON COLUMN recommendations.recommendedPeer IS 'Ник пира, к которому рекомендуют идти на проверку';

-- Таблица XP
create table if not exists xp (
	id uuid primary key default uuid_generate_v1(),
    "check" uuid, constraint fk_xp_check foreign key ("check") references checks(id),
    xpAmount integer
);
COMMENT ON COLUMN xp."check" IS 'ID проверки';
COMMENT ON COLUMN xp.xpAmount IS 'Количество полученного XP';

-- Состояние пира при посещении кампуса (1 - пришел, 2 - вышел)
create type peer_visit_state as enum ('1', '2');

-- Таблица TimeTracking
create table if not exists timetracking (
    id uuid primary key default uuid_generate_v1(),
    peer varchar, constraint fk_timetracking_peer foreign key (peer) references peers(nickname),
    "date" date,
    "time" TIME without time zone,
    state peer_visit_state
);
COMMENT ON COLUMN timetracking.peer IS 'Ник пира';
COMMENT ON COLUMN timetracking."date" IS 'Дата';
COMMENT ON COLUMN timetracking."time" IS 'Время';
COMMENT ON COLUMN timetracking.state IS 'Состояние (1 - пришел, 2 - вышел)';

-- Триггер: перед добавлением записи в таблицу, проверить что соблюдена последовательность значений
CREATE OR REPLACE FUNCTION fnc_trg_timetracking_check_on_insert() RETURNS trigger AS $$
DECLARE
    count_in integer := 0;
    count_out integer := 0;
	last_in_time time := CURRENT_TIME(1);
BEGIN
	count_in := (SELECT count(id) FROM timetracking WHERE state = '1' AND peer = NEW.peer AND "date" = NEW."date");
	count_out := (SELECT count(id) FROM timetracking WHERE state = '2' AND peer = NEW.peer AND "date" = NEW."date");
	last_in_time := (SELECT "time" FROM timetracking WHERE "date" = NEW."date" AND peer = NEW.peer AND state = '1' ORDER BY time DESC LIMIT 1);
	
	-- проверяем что выход происходит позже последнего входа в тот же день
    IF NEW.state = '2' AND NEW."time" >= last_in_time AND count_in >= 1 THEN
        RETURN NEW;
	-- проверяем что количество входов соответствует количеству выходов
    ELSIF NEW.state = '1' AND count_in = count_out THEN
       	RETURN NEW;
    ELSE
       	raise exception 'insert error Peer: % state: % date: % time: %', NEW.peer, NEW.state, NEW."date", NEW."time";
       	RETURN NULL;
    END IF;
END;
$$ LANGUAGE plpgsql;

CREATE OR REPLACE TRIGGER trg_timetracking_check_on_insert BEFORE INSERT ON timetracking
FOR EACH ROW EXECUTE PROCEDURE fnc_trg_timetracking_check_on_insert();

-- Функция для возвращения названия колонок таблицы без колонки ID
CREATE OR REPLACE FUNCTION fnc_get_columns_names(tablename TEXT) RETURNS TEXT AS $$
BEGIN
RETURN (SELECT string_agg(quote_ident(column_name), ',') FROM (select column_name from information_schema.columns WHERE table_name = tablename AND column_name != 'id' ORDER BY ordinal_position) as col);
END;
$$ LANGUAGE plpgsql;

-- Процедура импорта данных из CSV
CREATE OR REPLACE PROCEDURE prc_import_data_from_csv(IN tablename text, IN path text, IN delim text DEFAULT ';')
LANGUAGE 'plpgsql'
AS $$
BEGIN
EXECUTE format('COPY %s(%s) FROM %L WITH (FORMAT csv, DELIMITER ''%s'', HEADER true)', tablename, fnc_get_columns_names(tablename), path, delim);
END;
$$;

-- Импорт данных о пирах из csv, тестирование процедуры. Путь к файлу на сервере.
-- CALL prc_import_data_from_csv('peers', '/home/peers_table.csv');

-- Процедура экспорта данных в CSV
CREATE OR REPLACE PROCEDURE prc_export_data_to_csv(IN tablename text, IN path text, IN delim text DEFAULT ';')
LANGUAGE 'plpgsql'
AS $$
BEGIN
EXECUTE format('COPY %s TO %L WITH (FORMAT csv, DELIMITER ''%s'', HEADER true)', tablename, path, delim);
END;
$$;

-- Экспорт данных о пирах в csv, тестирование процедуры. Путь к файлу на сервере.
-- У пользователя, от которого запущен сервер должны быть права на запись в указанный каталог
-- CALL prc_export_data_to_csv('peers', '/home/peers_table_export.csv');

-- Наполнение данными
-- TRUNCATE peers CASCADE;
INSERT INTO peers(nickname, birthday)
VALUES ('canonar', '1998-03-17'), ('varanas', '1996-03-04'), ('wanayn', '1986-11-27'),
('lakynari', '1999-01-30'), ('gasveni', '1995-05-12'), ('filant', '1991-08-29'),
('ohomim', '2000-05-14'), ('dotorucog', '1992-02-21'), ('jevusihavup', '1974-11-14'),
('wuzitaloqyk', '1975-08-23'), ('mayflower', '1983-07-05'), ('pralltiller', '2004-05-15'),
('quern', '1989-07-20'), ('helixo', '1980-04-21'), ('ultralex', '2000-08-30'),
('henrikom', '1996-03-07'), ('geenateliai', '1994-10-18'), ('quintent', '1991-10-29'),
('mrodem', '1994-06-06'), ('chanhaaa', '1979-11-02'), ('primat', '2003-03-22'),
('barbin', '1996-03-15'), ('ratarahaveni', '1989-11-09'), ('inester', '1990-12-17'),
('mmyaak', '1995-03-22');

-- TRUNCATE tasks CASCADE;
INSERT INTO tasks(title, "parentTask", "maxXP")
VALUES ('C1_Pool', NULL, 3000), ('C2_SimpleBashUtils', 'C1_Pool', 350),
('C3_Stringplus', 'C2_SimpleBashUtils', 690), ('C4_Math', 'C2_SimpleBashUtils', 300),
('C5_Decimal', 'C2_SimpleBashUtils', 350), ('C6_Matrix', 'C5_Decimal', 200),
('C7_SmartCalc_v1', 'C6_Matrix', 650), ('C8_3DViewer_v1', 'C7_SmartCalc_v1', 1043),
('D01_Linux', 'C2_SimpleBashUtils', 300), ('D02_Linux_Nexwork', 'D01_Linux', 350),
('D03_LinuxMonitoring_v1', 'D02_Linux_Nexwork', 350),
('D04_LinuxMonitoring_v2', 'D03_LinuxMonitoring_v1', 350), 
('D05_SimpleDocker', 'D03_LinuxMonitoring_v1', 300), ('D06_CICD', 'D05_SimpleDocker', 300),
('CPP1_s21_matrix', 'C8_3DViewer_v1', 300), ('CPP2_s21_containers', 'CPP1_s21_matrix', 501),
('CPP3_SmartCalc_v2', 'CPP2_s21_containers', 750), ('CPP4_3DViewer_v2', 'CPP3_SmartCalc_v2', 1050),
('CPP5_3DViewer_v2.1', 'CPP4_3DViewer_v2', 600), ('CPP6_3DViewer_v2.2', 'CPP4_3DViewer_v2', 600),
('CPP7_MLP', 'CPP4_3DViewer_v2', 700), ('CPP8_PhotoLab_v1.0', 'CPP4_3DViewer_v2', 450),
('CPP9_MonitoringSystem', 'CPP4_3DViewer_v2', 450);

-- TRUNCATE checks CASCADE;
INSERT INTO checks(id, peer, task, date)
VALUES ('f5e7fc36-8e8f-11ed-9a42-0242ac110002', 'mayflower', 'C2_SimpleBashUtils', '2022-12-07'),
('2119a4c2-8e90-11ed-9a42-0242ac110002', 'quern', 'C2_SimpleBashUtils', '2022-12-07'),
('67938738-8e90-11ed-9a42-0242ac110002', 'inester', 'C2_SimpleBashUtils', '2022-12-07'),
('775c7d28-8e90-11ed-9a42-0242ac110002', 'inester', 'C2_SimpleBashUtils', '2022-12-08'),
('9497f142-8e90-11ed-9a42-0242ac110002', 'mrodem', 'C2_SimpleBashUtils', '2022-12-09'),
('acf21dda-8e90-11ed-9a42-0242ac110002', 'lakynari', 'C2_SimpleBashUtils', '2022-12-10'),
('d1f8f04a-8e90-11ed-9a42-0242ac110002', 'ratarahaveni', 'C2_SimpleBashUtils', '2022-12-10'),
('35ddd654-8e9e-11ed-9a42-0242ac110002', 'ultralex', 'C2_SimpleBashUtils', '2022-12-10'),
('3d0eb2b8-8e9e-11ed-9a42-0242ac110002', 'ultralex', 'C2_SimpleBashUtils', '2022-12-11'),
('57140ff0-8e9e-11ed-9a42-0242ac110002', 'ultralex', 'C2_SimpleBashUtils', '2022-12-12'),
('67904880-8e9e-11ed-9a42-0242ac110002', 'ultralex', 'C2_SimpleBashUtils', '2022-12-12'),
('80508006-8e9e-11ed-9a42-0242ac110002', 'jevusihavup', 'C2_SimpleBashUtils', '2022-12-12'),
('8d937980-8e9e-11ed-9a42-0242ac110002', 'jevusihavup', 'C2_SimpleBashUtils', '2022-12-14'),
('ccd6f0d6-8e9e-11ed-9a42-0242ac110002', 'ratarahaveni', 'C2_SimpleBashUtils', '2022-12-14'),
('fd4215c0-8e9e-11ed-9a42-0242ac110002', 'ratarahaveni', 'C2_SimpleBashUtils', '2022-12-14'),
('2443a17a-8e9f-11ed-9a42-0242ac110002', 'ratarahaveni', 'C3_Stringplus', '2022-12-22'),
('0866c2d0-8eda-11ed-9a42-0242ac110002', 'quern', 'C3_Stringplus', '2022-12-23'),
('4f44caee-8eda-11ed-9a42-0242ac110002', 'inester', 'C3_Stringplus', '2022-12-23'),
('88a20f90-8eda-11ed-9a42-0242ac110002', 'mrodem', 'C3_Stringplus', '2022-12-24'),
('92b7a030-8eda-11ed-9a42-0242ac110002', 'mrodem', 'C3_Stringplus', '2022-12-25'),
('aa3dc842-8eda-11ed-9a42-0242ac110002', 'mrodem', 'C3_Stringplus', '2022-12-27');

-- TRUNCATE p2p;
INSERT INTO p2p("check", "checkingPeer", state, "time")
VALUES ('f5e7fc36-8e8f-11ed-9a42-0242ac110002', 'wanayn', 'Start', '13:34:07'),
('f5e7fc36-8e8f-11ed-9a42-0242ac110002', 'wanayn', 'Failure', '13:44:23'),
('2119a4c2-8e90-11ed-9a42-0242ac110002', 'wuzitaloqyk', 'Start', '13:35:20'),
('2119a4c2-8e90-11ed-9a42-0242ac110002', 'wuzitaloqyk', 'Success', '13:45:27'),
('67938738-8e90-11ed-9a42-0242ac110002', 'gasveni', 'Start', '13:37:18'),
('67938738-8e90-11ed-9a42-0242ac110002', 'gasveni', 'Failure', '13:47:21'),
('775c7d28-8e90-11ed-9a42-0242ac110002', 'gasveni', 'Start', '13:37:44'),
('775c7d28-8e90-11ed-9a42-0242ac110002', 'gasveni', 'Success', '13:47:52'),
('9497f142-8e90-11ed-9a42-0242ac110002', 'filant', 'Start', '13:38:33'),
('9497f142-8e90-11ed-9a42-0242ac110002', 'filant', 'Success', '13:48:38'),
('acf21dda-8e90-11ed-9a42-0242ac110002', 'pralltiller', 'Start', '13:39:14'),
('acf21dda-8e90-11ed-9a42-0242ac110002', 'pralltiller', 'Success', '13:49:17'),
('d1f8f04a-8e90-11ed-9a42-0242ac110002', 'quintent', 'Start', '13:40:16'),
('d1f8f04a-8e90-11ed-9a42-0242ac110002', 'quintent', 'Success', '13:50:20'),
('35ddd654-8e9e-11ed-9a42-0242ac110002', 'filant', 'Start', '15:16:07'),
('35ddd654-8e9e-11ed-9a42-0242ac110002', 'filant', 'Failure', '15:26:12'),
('3d0eb2b8-8e9e-11ed-9a42-0242ac110002', 'filant', 'Start', '15:16:19'),
('3d0eb2b8-8e9e-11ed-9a42-0242ac110002', 'filant', 'Failure', '15:26:23'),
('57140ff0-8e9e-11ed-9a42-0242ac110002', 'lakynari', 'Start', '15:17:03'),
('57140ff0-8e9e-11ed-9a42-0242ac110002', 'lakynari', 'Failure', '15:27:11'),
('67904880-8e9e-11ed-9a42-0242ac110002', 'chanhaaa', 'Start', '15:17:31'),
('67904880-8e9e-11ed-9a42-0242ac110002', 'chanhaaa', 'Success', '15:27:40'),
('80508006-8e9e-11ed-9a42-0242ac110002', 'chanhaaa', 'Start', '15:18:12'),
('80508006-8e9e-11ed-9a42-0242ac110002', 'chanhaaa', 'Failure', '15:28:23'),
('8d937980-8e9e-11ed-9a42-0242ac110002', 'pralltiller', 'Start', '15:18:35'),
('8d937980-8e9e-11ed-9a42-0242ac110002', 'pralltiller', 'Success', '15:28:41'),
('ccd6f0d6-8e9e-11ed-9a42-0242ac110002', 'primat', 'Start', '15:20:21'),
('ccd6f0d6-8e9e-11ed-9a42-0242ac110002', 'primat', 'Failure', '15:31:03'),
('fd4215c0-8e9e-11ed-9a42-0242ac110002', 'henrikom', 'Start', '15:21:42'),
('fd4215c0-8e9e-11ed-9a42-0242ac110002', 'henrikom', 'Success', '15:31:46'),
('2443a17a-8e9f-11ed-9a42-0242ac110002', 'primat', 'Start', '15:22:47'),
('2443a17a-8e9f-11ed-9a42-0242ac110002', 'primat', 'Success', '15:32:53'),
('0866c2d0-8eda-11ed-9a42-0242ac110002', 'primat', 'Start', '22:24:21'),
('0866c2d0-8eda-11ed-9a42-0242ac110002', 'primat', 'Failure', '22:34:26'),
('4f44caee-8eda-11ed-9a42-0242ac110002', 'mmyaak', 'Start', '22:26:20'),
('4f44caee-8eda-11ed-9a42-0242ac110002', 'mmyaak', 'Success', '22:36:20'),
('88a20f90-8eda-11ed-9a42-0242ac110002', 'mmyaak', 'Start', '22:27:56'),
('88a20f90-8eda-11ed-9a42-0242ac110002', 'mmyaak', 'Failure', '22:38:09'),
('92b7a030-8eda-11ed-9a42-0242ac110002', 'mmyaak', 'Start', '22:28:13'),
('92b7a030-8eda-11ed-9a42-0242ac110002', 'mmyaak', 'Success', '22:38:19'),
('aa3dc842-8eda-11ed-9a42-0242ac110002', 'helixo', 'Start', '22:28:52'),
('aa3dc842-8eda-11ed-9a42-0242ac110002', 'helixo', 'Success', '22:38:52');

-- TRUNCATE verter;
INSERT INTO verter("check", state, "time")
VALUES ('2119a4c2-8e90-11ed-9a42-0242ac110002', 'Start', '13:45:31'),
('2119a4c2-8e90-11ed-9a42-0242ac110002', 'Failure', '13:46:21'),
('775c7d28-8e90-11ed-9a42-0242ac110002', 'Start', '13:48:08'),
('775c7d28-8e90-11ed-9a42-0242ac110002', 'Success', '13:48:58'),
('9497f142-8e90-11ed-9a42-0242ac110002', 'Start', '13:49:41'),
('9497f142-8e90-11ed-9a42-0242ac110002', 'Success', '13:50:41'),
('acf21dda-8e90-11ed-9a42-0242ac110002', 'Start', '14:00:53'),
('acf21dda-8e90-11ed-9a42-0242ac110002', 'Success', '14:01:53'),
('d1f8f04a-8e90-11ed-9a42-0242ac110002', 'Start', '13:50:20'),
('d1f8f04a-8e90-11ed-9a42-0242ac110002', 'Failure', '18:50:20'),
('67904880-8e9e-11ed-9a42-0242ac110002', 'Start', '22:19:35'),
('67904880-8e9e-11ed-9a42-0242ac110002', 'Success', '22:19:35'),
('8d937980-8e9e-11ed-9a42-0242ac110002', 'Start', '22:20:04'),
('8d937980-8e9e-11ed-9a42-0242ac110002', 'Success', '22:20:04'),
('fd4215c0-8e9e-11ed-9a42-0242ac110002', 'Start', '22:20:12'),
('fd4215c0-8e9e-11ed-9a42-0242ac110002', 'Success', '22:20:12'),
('2443a17a-8e9f-11ed-9a42-0242ac110002', 'Start', '22:20:26'),
('2443a17a-8e9f-11ed-9a42-0242ac110002', 'Success', '22:20:26'),
('4f44caee-8eda-11ed-9a42-0242ac110002', 'Start', '22:26:50'),
('4f44caee-8eda-11ed-9a42-0242ac110002', 'Success', '22:26:50'),
('92b7a030-8eda-11ed-9a42-0242ac110002', 'Start', '22:28:37'),
('92b7a030-8eda-11ed-9a42-0242ac110002', 'Failure', '22:28:43'),
('aa3dc842-8eda-11ed-9a42-0242ac110002', 'Start', '22:29:03'),
('aa3dc842-8eda-11ed-9a42-0242ac110002', 'Success', '22:29:03');

-- TRUNCATE transferredpoints;
INSERT INTO transferredpoints("checkingPeer", "checkedPeer", "pointsAmount")
VALUES ('wanayn', 'mayflower', 1), ('wuzitaloqyk', 'quern', 1), ('gasveni', 'inester', 2),
('filant', 'mrodem', 1), ('pralltiller', 'lakynari', 1), ('quintent', 'ratarahaveni', 1),
('filant', 'ultralex', 2), ('lakynari', 'ultralex', 1), ('chanhaaa', 'ultralex', 1),
('chanhaaa', 'jevusihavup', 1), ('pralltiller', 'jevusihavup', 1), ('henrikom', 'ratarahaveni', 1),
('primat', 'ratarahaveni', 2), ('primat', 'quern', 1), ('mmyaak', 'inester', 1),
('mmyaak', 'mrodem', 2), ('helixo', 'mrodem', 1);

-- TRUNCATE friends;
INSERT INTO friends(peer1, peer2)
VALUES ('wanayn', 'mayflower'), ('gasveni', 'inester'), ('filant', 'ultralex'),
('primat', 'ratarahaveni'), ('mmyaak', 'mrodem');

-- TRUNCATE recommendations;
INSERT INTO recommendations(peer, recommendedPeer)
VALUES ('jevusihavup', 'geenateliai'), ('geenateliai', 'mrodem'), ('canonar', 'inester'),
('dotorucog', 'mayflower'), ('barbin', 'ratarahaveni'), ('ultralex', 'ratarahaveni'),
('geenateliai', 'ratarahaveni'), ('varanas', 'ratarahaveni'), ('ohomim', 'primat');

-- TRUNCATE xp;
INSERT INTO xp("check", xpAmount)
VALUES ('775c7d28-8e90-11ed-9a42-0242ac110002', 350),
('9497f142-8e90-11ed-9a42-0242ac110002', 292),
('acf21dda-8e90-11ed-9a42-0242ac110002', 300),
('67904880-8e9e-11ed-9a42-0242ac110002', 334),
('8d937980-8e9e-11ed-9a42-0242ac110002', 350),
('fd4215c0-8e9e-11ed-9a42-0242ac110002', 350),
('2443a17a-8e9f-11ed-9a42-0242ac110002', 690),
('4f44caee-8eda-11ed-9a42-0242ac110002', 690),
('aa3dc842-8eda-11ed-9a42-0242ac110002', 624);

-- TRUNCATE timetracking;
INSERT INTO timetracking(peer, date, "time", state)
VALUES ('helixo', '2022-12-03', '08:02', '1'), ('quintent', '2022-12-03', '08:28', '1'),
('wanayn', '2022-12-03', '09:15', '1'), ('mmyaak', '2022-12-03', '10:02', '1'),
('ratarahaveni', '2022-12-03', '10:57', '1'), ('wanayn', '2022-12-03', '11:26', '2'),
('primat', '2022-12-03', '11:32', '1'), ('helixo', '2022-12-03', '11:37', '2'),
('quintent', '2022-12-03', '11:37', '2'), ('wanayn', '2022-12-03', '12:04', '1'),
('mmyaak', '2022-12-03', '12:25', '2'), ('ratarahaveni', '2022-12-03', '12:26', '2'),
('helixo', '2022-12-03', '12:40', '1'), ('quintent', '2022-12-03', '12:40', '1'),
('mmyaak', '2022-12-03', '14:22', '1'), ('ratarahaveni', '2022-12-03', '15:23', '1'),
('mayflower', '2022-12-03', '16:02', '1'), ('helixo', '2022-12-03', '18:27', '2'),
('wanayn', '2022-12-03', '18:29', '2'), ('quintent', '2022-12-03', '18:54', '2'),
('primat', '2022-12-03', '19:32', '2'), ('mmyaak', '2022-12-03', '21:46', '2'),
('ratarahaveni', '2022-12-03', '23:26', '2'), ('mayflower', '2022-12-03', '23:43', '2'),
('helixo', '2022-12-04', '11:37', '1'), ('mayflower', '2022-12-04', '14:13', '1'),
('primat', '2022-12-04', '14:27', '1'), ('quintent', '2022-12-04', '15:28', '1'),
('mayflower', '2022-12-04', '17:29', '2'), ('mayflower', '2022-12-04', '17:37', '1'),
('primat', '2022-12-04', '19:04', '2'), ('helixo', '2022-12-04', '20:44', '2'),
('quintent', '2022-12-04', '21:38', '2'), ('mayflower', '2022-12-04', '22:38', '2'),
('mmyaak', '2022-12-05', '13:25', '1'), ('wanayn', '2022-12-05', '14:04', '1'),
('mmyaak', '2022-12-05', '15:12', '2'), ('mmyaak', '2022-12-05', '15:20', '1'),
('wanayn', '2022-12-05', '17:47', '2'), ('mmyaak', '2022-12-05', '21:50', '2'),
('helixo', '2023-01-03', '09:23', '1'), ('helixo', '2023-01-03', '17:04', '2'),
('mayflower', '2023-01-04', '12:13', '1'), ('quintent', '2023-01-04', '08:28', '1'),
('wanayn', '2023-01-04', '09:15', '1'), ('mmyaak', '2023-01-04', '10:02', '1'),
('mayflower', '2023-01-04', '19:25', '2'), ('quintent', '2023-01-04', '15:18', '2'),
('wanayn', '2023-01-04', '16:15', '2'), ('mmyaak', '2023-01-04', '20:04', '2');
