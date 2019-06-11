delimiter //
create trigger akj before update on student_marks
for each row
begin
	set new.total = new.sub1 + new.sub2 + new.sub3 + new.sub4 + new.sub5;
	set new.per_marks = new.total/500 * 100;
	
	if new.per_marks >= 90
	then set new.grade='A';
	
	elseif new.per_marks >= 80
	then set new.grade = 'B';
	
	elseif new.per_marks >= 70
	then set new.grade = 'C';

	elseif new.per_marks >= 60
	then set new.grade = 'D';

	else set new.grade = 'E';

	end if;
end;//
delimiter ;



delimiter //
create trigger depositor before delete on customer
for each row
begin
	if(old.cname in (select cname from depositor))
	then insert into customer values(old.cid, old.cname, old.address);
	end if;
end;//
delimiter ;

delimiter //
create trigger depositor1 before delete on customer
for each row
begin
	if(old.cname in (select cname from depositor))
	then delete from depositor where old.name=depositor.name;
	end if;
end;//
delimiter ;
