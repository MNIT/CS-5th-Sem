/*Retrieve the name of each employee who has a dependent with the same first name and same sex as the 
employee.  */

SELECT E.FNAME, E.LNAME
FROM EMPLOYEE AS E
WHERE E.SSN IN ( SELECT ESSN
                FROM DEPENDENT
                WHERE E.FNAME=DEPENDENT_NAME AND E.SEX=SEX);


SELECT E.FNAME, E.LNAME
FROM EMPLOYEE AS E, DEPENDENT AS D
WHERE E.SSN=D.ESSN AND
E.FNAME = D.DEPENDENT_NAME AND
E.SEX=D.SEX;

SELECT E.FNAME, E.LNAME
FROM EMPLOYEE AS E
WHERE EXISTS ( SELECT *
             FROM DEPENDENT
             WHERE E.SSN=ESSN AND
             E.FNAME=DEPENDENT_NAME AND
             E.SEX=SEX );
             
/* Retrieve the names of employees who have no dependents */
SELECT FNAME, LNAME  
FROM EMPLOYEE  
WHERE NOT EXISTS (
				SELECT * FROM DEPENDENT  
				WHERE SSN=ESSN);  


/* retrieve the name and address of every employee who works for 
the ‘Research’ department */

SELECT FNAME, LNAME, ADDRESS  
FROM (EMPLOYEE
		JOIN DEPARTMENT 
		ON DNO=DNUMBER)  
		WHERE DNAME=‘Research’;  
		

/* For each employee, retrieve the employee’s first and last name and the first and last name of his or her 
immediate supervisor */
SELECT E.LNAME AS EMPLOYEE_NAME, S.LNAME AS SUPERVISOR_NAME  
FROM (EMPLOYEE 	AS E 
	LEFT OUTER JOIN EMPLOYEE AS S 
	ON E.SUPERSSN=S.SSN);  


SELECT * FROM (PROJECT INNER JOIN WORKS_ON ON PNUMBER=PNO);

SELECT * FROM (PROJECT FULL JOIN WORKS_ON ON PNUMBER=PNO);

/* For every project located in ‘Stafford’, list the project number, the controlling department number, and 
the department manager’s last name, address, and birthdate. */

SELECT PNUMBER, DNUM, LNAME, ADDRESS, BDATE
FROM (
 (
	PROJECT
	JOIN DEPARTMENT ON DNUM = DNUMBER
 )
JOIN EMPLOYEE ON MGRSSN = SSN
)
WHERE PLOCATION = 'stafford'
















