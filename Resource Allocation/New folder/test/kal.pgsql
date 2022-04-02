-- number of instructors teaching courses of a particular department
select dept_name, (select count(distinct teaches.ins_id) from teaches, course where teaches.course_id = course.courseid and course.dept_name = department.dept_name) from department;

-- students did not registering for any cse cources
select * from student where not exists(select  * from course, takes where course.courseid = takes.course_id and takes.studentid = student.studentid and course.dept_name = 'CSE');

-- number of times a course was offered 
select c1.courseid, (select count(distinct (ins_id, semester, year)) from teaches t2 where t2.course_id = c1.courseid) from course c1;

-- for each teacher how many students
select i1.insid, (select count(distinct studentid) from takes, teaches where takes.course_id = teaches.course_id and teaches.section_id = takes.section_id and teaches.semester = takes.semester and takes.year = teaches.year and teaches.ins_id = i1.insid) from instructor i1;
 

select distinct i1.insid, i1.dept_name 
from instructor i1
where exists(
    select * 
    from teaches, course 
    where teaches.ins_id = i1.insid 
    and teaches.course_id = course.courseid 
    and i1.dept_name = course.dept_name);


select i1.insid, i1.dept_name,(
    select 
    count(distinct (course_id, section_id, semester, year)) 
    from teaches t2, course c2 
    where teaches.course_id = course.courseid
    and teaches.ins_id = i1.insid
) 
from instructor i1