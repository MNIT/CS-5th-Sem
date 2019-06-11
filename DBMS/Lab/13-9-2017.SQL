/*  1. Print First name and Last name of all employees.*/
SELECT CONCAT(FNAME, " ", LNAME) AS FULL_NAME FROM EMPLOYEE;

/*  2. Find age and day of birth from 'Employee' table whose First and Last name begins with B and has at least 5 characters.*/
SELECT FNAME, LNAME,
EXTRACT(YEAR FROM CURRENT_DATE) - EXTRACT(YEAR FROM BDATE) AS AGE,
DAYNAME(BDATE),
MONTHNAME(BDATE),
EXTRACT(YEAR FROM BDATE) AS YEAR
FROM EMPLOYEE
WHERE FNAME LIKE 'B____%' AND LNAME LIKE 'B____%';

/*  3. Print TOB of employee in 12hr format */
SELECT TIME_FORMAT(TOB, "%r")
FROM EMPLOYEE;



/*  1. Find all the employees who have more than the average salary of all employees of their department */

SELECT FNAME, SALARY FROM EMPLOYEE
WHERE SALARY > (SELECT AVG(SALARY) FROM EMPLOYEE AS E, DEPARTMENT AS D WHERE E.DNO=D.DNUMBER);

/*  2. Find all the departmants located in every city in which research department is located. */

SELECT DISTINCT DNAME, DEPARTMENT.DNUMBER
FROM DEPARTMENT,DEP_LOCATIONS
WHERE DNAME!='Research' AND DEPARTMENT.DNUMBER=DEP_LOCATIONS.DNUMBER AND DLOCATION IN (SELECT DLOCATION FROM DEPARTMENT,DEP_LOCATIONS WHERE DNAME='Research' AND DEPARTMENT.DNUMBER=DEP_LOCATIONS.DNUMBER);

/*  3. Find the department that has the smallest average payroll. */

SELECT DNO,AVG(SALARY) AS S
FROM EMPLOYEE
GROUP BY DNO
ORDER BY S
LIMIT 1;

SELECT DNUMBER,DNAME
FROM DEPARTMENT,EMPLOYEE
WHERE DNO=DNUMBER
GROUP BY DNUMBER
HAVING AVG(SALARY) <= ALL( SELECT AVG(SALARY)
FROM EMPLOYEE
GROUP BY DNO);

/*  4. Find those departments whose employees earn a higher salary, on average, than the average salary at the research department. */

SELECT DNUMBER,DNAME
FROM DEPARTMENT,EMPLOYEE
WHERE DNO=DNUMBER
GROUP BY DNUMBER
HAVING AVG(SALARY) > ( SELECT AVG(SALARY)
FROM EMPLOYEE, DEPARTMENT
WHERE DNO=DNUMBER AND DNAME='Research');









