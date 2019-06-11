-- phpMyAdmin SQL Dump
-- version 4.0.10deb1
-- http://www.phpmyadmin.net
--
-- Host: localhost
-- Generation Time: Aug 21, 2017 at 10:05 PM
-- Server version: 5.5.55-0ubuntu0.14.04.1
-- PHP Version: 5.5.9-1ubuntu4.21

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `bank_lavkush`
--

-- --------------------------------------------------------

--
-- Table structure for table `account`
--

CREATE TABLE IF NOT EXISTS `account` (
  `account_number` int(15) NOT NULL,
  `branch_name` varchar(15) NOT NULL,
  `balance` int(15) NOT NULL,
  PRIMARY KEY (`account_number`),
  KEY `b_name` (`branch_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `account`
--

INSERT INTO `account` (`account_number`, `branch_name`, `balance`) VALUES
(996, 'icici', 50000),
(997, 'icici', 20000),
(998, 'sbi', 10000),
(999, 'sbi', 10000);

-- --------------------------------------------------------

--
-- Table structure for table `borrower`
--

CREATE TABLE IF NOT EXISTS `borrower` (
  `customer_name` varchar(15) NOT NULL,
  `loan_number` int(15) NOT NULL,
  PRIMARY KEY (`customer_name`,`loan_number`),
  KEY `EMPDEPTFK` (`loan_number`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `borrower`
--

INSERT INTO `borrower` (`customer_name`, `loan_number`) VALUES
('lav', 111),
('kush', 112),
('sawant', 113),
('prince', 114);

-- --------------------------------------------------------

--
-- Table structure for table `branch`
--

CREATE TABLE IF NOT EXISTS `branch` (
  `branch_name` varchar(15) NOT NULL,
  `assets` int(6) NOT NULL,
  `branch_city` varchar(15) NOT NULL,
  PRIMARY KEY (`branch_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `branch`
--

INSERT INTO `branch` (`branch_name`, `assets`, `branch_city`) VALUES
('axis', 1000000, 'jaipur'),
('icici', 100000, 'jodhpur'),
('pnb', 100000, 'jaipur'),
('sbi', 100000, '100000');

-- --------------------------------------------------------

--
-- Table structure for table `customer`
--

CREATE TABLE IF NOT EXISTS `customer` (
  `customer_name` varchar(15) NOT NULL,
  `customer_street` char(15) DEFAULT NULL,
  `customer_city` varchar(15) NOT NULL,
  PRIMARY KEY (`customer_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `customer`
--

INSERT INTO `customer` (`customer_name`, `customer_street`, `customer_city`) VALUES
('kush', 'tonk_road', 'jodhpur'),
('lav', 'jwahar', 'jaipur'),
('prince', 'ajmeri', 'jaipur'),
('sawant', 'GT', 'jodhpur');

-- --------------------------------------------------------

--
-- Table structure for table `depositor`
--

CREATE TABLE IF NOT EXISTS `depositor` (
  `account_number` int(15) NOT NULL,
  `customer_name` varchar(15) NOT NULL,
  PRIMARY KEY (`customer_name`,`account_number`),
  KEY `acc` (`account_number`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `depositor`
--

INSERT INTO `depositor` (`account_number`, `customer_name`) VALUES
(996, 'prince'),
(997, 'sawant'),
(998, 'kush'),
(999, 'lav');

-- --------------------------------------------------------

--
-- Table structure for table `loan`
--

CREATE TABLE IF NOT EXISTS `loan` (
  `loan_number` int(15) NOT NULL,
  `branch_name` varchar(15) NOT NULL,
  `amount` int(15) NOT NULL,
  PRIMARY KEY (`loan_number`),
  KEY `b_name1` (`branch_name`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Dumping data for table `loan`
--

INSERT INTO `loan` (`loan_number`, `branch_name`, `amount`) VALUES
(111, 'sbi', 10000),
(112, 'sbi', 20000),
(113, 'icici', 50000),
(114, 'icici', 10000);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `account`
--
ALTER TABLE `account`
  ADD CONSTRAINT `b_name` FOREIGN KEY (`branch_name`) REFERENCES `branch` (`branch_NAME`);

--
-- Constraints for table `borrower`
--
ALTER TABLE `borrower`
  ADD CONSTRAINT `c_name` FOREIGN KEY (`customer_name`) REFERENCES `customer` (`customer_name`),
  ADD CONSTRAINT `EMPDEPTFK` FOREIGN KEY (`loan_number`) REFERENCES `loan` (`loan_number`);

--
-- Constraints for table `depositor`
--
ALTER TABLE `depositor`
  ADD CONSTRAINT `cust` FOREIGN KEY (`customer_name`) REFERENCES `customer` (`customer_name`),
  ADD CONSTRAINT `acc` FOREIGN KEY (`account_number`) REFERENCES `account` (`account_number`);

--
-- Constraints for table `loan`
--
ALTER TABLE `loan`
  ADD CONSTRAINT `b_name1` FOREIGN KEY (`branch_name`) REFERENCES `branch` (`branch_NAME`);

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
