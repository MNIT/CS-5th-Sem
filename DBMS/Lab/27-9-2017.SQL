/*
	AUTHOR : ADARSH JAIN (2015UCP1547)
*/


SELECT * FROM(Q1 JOIN Q3 ON Q1.PNUMBER=Q3.PNUMBER);

CREATE INDEX FNAME1 ON EMPLOYEE(FNAME);

CREATE VIEW PROJECT_VIEW AS
SELECT *
FROM PROJECT
WHERE PNAME IS NOT NULL
WITH CHECK OPTION;

INSERT INTO PROJECT_VIEW(PNUMBER, PLOCATION) VALUES(4, 'JAIPUR');


CREATE VIEW WITH1 AS
SELECT *
FROM PROJECT
WHERE PNAME='ProductX'
WITH CHECK OPTION;

-- DOES NOT WORK
UPDATE WITH1 SET PNAME='QW';
INSERT INTO WITH1(PNAME, PNUMBER) VALUES('ProductY', 1);

-- WILL WORK
INSERT INTO WITH1(PNAME, PNUMBER, PLOCATION, DNUM) VALUES('ProductX', 1, 'Jaipur', 41);

/*
	INDEXING
*/
-- 0.0063 sec
select place_name
from places
where country_code='IN';

-- 1.4560 sec
create index country on places(country_code)

-- creating index on country_code

-- 0.0004 sec
select place_name
from places
where country_code='IN';

-- 0.0916 sec
SELECT * FROM `places` WHERE latitude>50 and postal_code=50000

-- 0.0827 sec (After Indexing)















