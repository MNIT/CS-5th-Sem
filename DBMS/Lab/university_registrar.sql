-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 09, 2017 at 10:07 PM
-- Server version: 5.5.55-0ubuntu0.14.04.1
-- PHP Version: 5.5.9-1ubuntu4.21

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `university_registrar`
--

-- --------------------------------------------------------

--
-- Table structure for table `courses`
--

CREATE TABLE IF NOT EXISTS `courses` (
  `course_number` int(11) NOT NULL,
  `title` varchar(50) NOT NULL,
  `credit` int(11) NOT NULL,
  `syllabus` varchar(255) DEFAULT NULL,
  `pre_requisite` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`course_number`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `courses`
--

INSERT INTO `courses` (`course_number`, `title`, `credit`, `syllabus`, `pre_requisite`) VALUES
(1, 'DBMS', 4, 'don''t know', 'programming'),
(2, 'DSA', 3, 'stack,tree', 'c programming'),
(3, 'DAA', 4, 'algorithms', 'DSA');

-- --------------------------------------------------------

--
-- Table structure for table `course_offering`
--

CREATE TABLE IF NOT EXISTS `course_offering` (
  `id` int(11) NOT NULL DEFAULT '0',
  `course_number` int(10) unsigned NOT NULL,
  `semester` int(10) unsigned NOT NULL,
  `section` char(1) NOT NULL,
  `instructor` int(11) NOT NULL,
  `classroom` int(11) NOT NULL,
  `year` int(11) NOT NULL,
  `timing` time NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `course_offering`
--

INSERT INTO `course_offering` (`id`, `course_number`, `semester`, `section`, `instructor`, `classroom`, `year`, `timing`) VALUES
(1, 2, 5, 'A', 4, 19, 2017, '16:00:00'),
(2, 3, 5, 'B', 1, 18, 2016, '00:00:12'),
(3, 2, 5, 'D', 1, 12, 2017, '12:00:00');

-- --------------------------------------------------------

--
-- Table structure for table `enroll`
--

CREATE TABLE IF NOT EXISTS `enroll` (
  `id` int(10) unsigned NOT NULL,
  `student_id` int(11) NOT NULL,
  `course_id` int(11) NOT NULL,
  `grades` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `enroll`
--

INSERT INTO `enroll` (`id`, `student_id`, `course_id`, `grades`) VALUES
(1, 2, 3, 12),
(2, 0, 1, 32),
(3, 0, 2, 12),
(4, 0, 2, 11),
(5, 0, 0, 10),
(6, 0, 1, 6);

-- --------------------------------------------------------

--
-- Table structure for table `instructor`
--

CREATE TABLE IF NOT EXISTS `instructor` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `mail_id` varchar(255) NOT NULL,
  `department` varchar(50) NOT NULL,
  `title` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB  DEFAULT CHARSET=latin1 AUTO_INCREMENT=5 ;

--
-- Dumping data for table `instructor`
--

INSERT INTO `instructor` (`id`, `name`, `mail_id`, `department`, `title`) VALUES
(1, 'adarsh', 'aj@gmail.com', 'computer science', 'Machine Learning'),
(2, 'D', 'd@gmail.com', 'ECE', 'Graph theory'),
(3, 'FD', 'srt@mnit.ac.in', 'CSE', 'Artificial Intelligence'),
(4, 'VD Sir', 'vimal@gmail.com', 'CSE', 'Cloud Computing');

-- --------------------------------------------------------

--
-- Table structure for table `offered`
--

CREATE TABLE IF NOT EXISTS `offered` (
  `id` int(10) unsigned NOT NULL,
  `course_id` int(11) NOT NULL,
  `course_number` int(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `offered`
--

INSERT INTO `offered` (`id`, `course_id`, `course_number`) VALUES
(1, 2, 3),
(2, 1, 2);

-- --------------------------------------------------------

--
-- Table structure for table `student`
--

CREATE TABLE IF NOT EXISTS `student` (
  `id` int(10) unsigned NOT NULL,
  `name` varchar(50) NOT NULL,
  `program` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `student`
--

INSERT INTO `student` (`id`, `name`, `program`) VALUES
(1, 'mohit', 'BTech'),
(2, 'saksham', 'MTech');

-- --------------------------------------------------------

--
-- Table structure for table `teaches`
--

CREATE TABLE IF NOT EXISTS `teaches` (
  `id` int(10) unsigned NOT NULL,
  `instructor_id` int(10) unsigned NOT NULL,
  `course_id` int(10) unsigned NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `teaches`
--

INSERT INTO `teaches` (`id`, `instructor_id`, `course_id`) VALUES
(1, 2, 3),
(2, 1, 2);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
