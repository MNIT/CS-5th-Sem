/* find the names of all branches in the loan relation */
select branch_name
from loan;

select loan_number, branch_name, amount * 100
from loan;

/*To find all loan number for loans made at the sbi branch
with loan amounts greater than $1200.*/
select loan_number
from loan
where branch_name = 'sbi' and amount > 12000;

/*Find the loan number of those loans with loan amounts
between $9000 and $100,000 (that is, >=$9000 and <=$100,000)*/
select loan_number
from loan
where amount between 9000 and 100000;

/*Find the Cartesian product borrower x loan*/
select *
from borrower, loan;

/*Find the name, loan number and loan amount of all customers
having a loan at the Perryridge branch.*/
select customer_name, borrower.loan_number, amount
from borrower, loan
where borrower.loan_number = loan.loan_number
and branch_name = 'sbi';

/*Find the name, loan number and loan amount of all customers;
rename the column name loan_number as loan_id.*/
select customer_name, borrower.loan_number as loan_id, amount
from borrower, loan
where borrower.loan_number = loan.loan_number;

/*Find the customer names and their loan numbers for all
customers having a loan at some branch.*/
select customer_name,t.loan_number, s.amount
from borrower as t, loan as s
where t.loan_number=s.loan_number;

/*Find the names of all branches that have greater assets than
some branch located in jaipur.*/
select distinct t.branch_name
from branch as t, branch as s
where t.assets>s.assets and s.branch_city='jaipur';

/*Find the names of all customers whose street includes the substring
“pur”.*/
select customer_name
from customer
where customer_street like '%pur%';

/*List in alphabetic order the names of all customers having a loan
in jaipur branch*/
select distinct customer_name
from borrower, loan
where borrower.loan_number = loan.loan_number
and branch_name = 'jaipur'
order by customer_name;

/*Find all customers who have a loan, an account, or both:*/
(select customer_name from depositor)
union
(select customer_name from borrower);

/*Find the average account balance at the icici branch.*/
select avg(balance)
from account
where branch_name='icici';

/*Find the number of tuples in the customer relation.*/
select count(*)
from customer;

/*Find the number of depositors in the bank.*/
select count(distinct customer_name)
from depositor;

/*Find the number of depositors for each branch.*/
select branch_name, count(distinct customer_name)
from account,depositor
where depositor.account_number = account.account_number
group by branch_name;

/*Find the names of all branches where the average account
balance is more than $1200.*/
SELECT branch_name, avg(balance)
FROM account
GROUP BY branch_name
HAVING avg(balance) > 1200;

/*Find all customers who have both an account and a loan at the
bank.*/
SELECT DISTINCT customer_name
FROM borrower
WHERE customer_name
IN (
	SELECT customer_name
	FROM depositor
	);

/*Find all customers who have a loan at the bank but do not have
an account at the bank*/
SELECT DISTINCT customer_name
FROM borrower
WHERE customer_name
NOT IN (
	SELECT customer_name
	FROM depositor
	);

/*Find all customers who have both an account and a loan at the
Perryridge branch*/
select distinct customer_name
from borrower, loan
where borrower.loan_number = loan.loan_number
and branch_name='sbi'
and (customer_name, branch_name)
in (
    select customer_name, branch_name
    from account, depositor
    where depositor.account_number = account.account_number
    );

/*Find all branches that have greater assets than some branch
located in Brooklyn.*/
select distinct t.branch_name
from branch as t, branch as s
where t.assets>s.assets
and s.branch_city='jaipur';

select branch_name
from branch
where assets > some
	(select assets
	from branch
	where branch_city = 'jaipur');
















