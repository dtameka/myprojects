-- Копирование базы данных для part4
-- Изменить datname = 'test' на название базы данных из part1
 SELECT pg_terminate_backend(pg_stat_activity.pid)
 FROM pg_stat_activity
 WHERE pg_stat_activity.datname = 'test' AND pid <> pg_backend_pid();

-- Изменить после TEMPLATE на имя базы данных из part1
 CREATE DATABASE testmeta WITH TEMPLATE test;

-- 1) Создать хранимую процедуру, которая, не уничтожая базу данных, уничтожает все те таблицы текущей базы данных, имена которых начинаются с фразы 'TableName'.
CREATE OR REPLACE PROCEDURE prc_remove_table_like(IN "tablename" varchar)
AS $$
DECLARE
    tablenames varchar;
BEGIN
FOR tablenames IN
    SELECT table_schema || '.' || '"' || table_name || '"'
    FROM   information_schema.tables
    WHERE  table_name LIKE tablename || '%'
    AND    table_schema NOT LIKE 'pg_%'
LOOP
EXECUTE 'DROP TABLE IF EXISTS ' || tablenames;
END LOOP;
END
-- Коментарий нужен для 4-го задания
-- YOU FOUND THIS
$$ LANGUAGE plpgsql;

-- TEST 1
-- Создаём таблицы
CREATE TABLE IF NOT EXISTS wow_peers ();
CREATE TABLE IF NOT EXISTS wow_ggggg ();
CREATE TABLE IF NOT EXISTS wow_tasks ();

-- Проверяем созданые таблицы
SELECT * FROM information_schema.tables
WHERE  table_schema NOT LIKE 'pg_%'
AND table_name LIKE 'wow' || '%'

-- Удаляем таблицы
BEGIN;
    CALL prc_remove_table_like('wow');
COMMIT;

-- Проверяем удалились ли таблицы
SELECT * FROM information_schema.tables
WHERE  table_schema NOT LIKE 'pg_%'
AND table_name LIKE 'wow' || '%'

-- 2) Создать хранимую процедуру с выходным параметром, которая выводит список имен и параметров всех скалярных SQL функций пользователя в текущей базе данных.
CREATE OR REPLACE PROCEDURE prс_get_names_and_parameters_func(IN ref refcursor, INOUT func_count integer = 0)
AS $$
BEGIN
OPEN ref SCROLL FOR
-- Имена функций и их параметры
with tmp AS (
SELECT r.routine_name, string_agg(p.parameter_name, ',')
FROM information_schema.routines AS r
JOIN information_schema.parameters p on r.specific_name = p.specific_name
WHERE routine_type='FUNCTION' AND r.specific_schema='public' AND p.parameter_mode='IN'
GROUP BY routine_name
) SELECT * FROM tmp;

MOVE FORWARD ALL FROM ref; -- перемещает курсор, не получая данные, MOVE
GET DIAGNOSTICS func_count := ROW_COUNT; -- число строк, обработанных последней командой SQL
MOVE BACKWARD ALL FROM ref; -- возвращаем курсор назад
 
END; 
$$ LANGUAGE plpgsql;

BEGIN;
CALL prс_get_names_and_parameters_func('cursor');
FETCH ALL IN "cursor";
COMMIT;

-- 3) Создать хранимую процедуру с выходным параметром, которая уничтожает все SQL DML триггеры в текущей базе данных.
CREATE OR REPLACE PROCEDURE prc_delete_dml_triggers(INOUT deleted_triggers INTEGER DEFAULT 0)
AS $$
DECLARE
    dml_statement text;
BEGIN
FOR dml_statement IN
    SELECT trigger_name || ' ON ' || event_object_table
    FROM information_schema.triggers
    WHERE trigger_schema = 'public'
    AND event_manipulation in('INSERT', 'DELETE', 'UPDATE')
LOOP
    EXECUTE format('DROP TRIGGER IF EXISTS ', dml_statement);
    deleted_triggers := deleted_triggers + 1;
END LOOP;
END;
$$ LANGUAGE plpgsql;

-- Смотрим наши триггеры
SELECT trigger_name
FROM information_schema.triggers; 

-- запускаем процедуру
BEGIN;
    CALL prc_delete_dml_triggers();	
	
	-- Проверяем что триггеры удалились
	SELECT trigger_name
	FROM information_schema.triggers;
ROLLBACK;

-- 4) Создать хранимую процедуру с входным параметром, которая выводит имена и описания типа объектов, 
--    в тексте которых на языке SQL встречается строка, задаваемая параметром процедуры.
CREATE or replace PROCEDURE prс_show_names_fnc_prc_like(IN ref refcursor, IN search_str varchar)
AS $$
BEGIN
OPEN ref FOR
    SELECT routine_name, routine_type, *
    FROM information_schema.routines
    WHERE specific_schema='public' AND routine_type IN ('FUNCTION', 'PROCEDURE')
    AND routine_definition LIKE '%' || search_str || '%';
END;
$$ LANGUAGE plpgsql;

BEGIN;
CALL prс_show_names_fnc_prc_like('cursor_name', 'YOU FOUND');
FETCH ALL IN "cursor_name";
ROLLBACK;
