CREATE DATABASE IF NOT EXISTS `cert_x509`;
USE `cert_x509`;

CREATE TABLE `certificates` (
	`id`	INTEGER NOT NULL,
	`common_name`	TEXT NOT NULL,
	`validity_start`	NUMERIC NOT NULL,
	`validity_end`	NUMERIC NOT NULL,
	`modulus`	TEXT NOT NULL,
	`certificate`	TEXT NOT NULL
);
