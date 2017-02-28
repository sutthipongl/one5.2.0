CREATE TABLE `ERF` (
    `filename` varchar(100) NOT NULL,
    `hash` varchar(65) DEFAULT NULL,
    `secret` varchar(65) NOT NULL,
    `lastupdate` datetime DEFAULT NULL,
    PRIMARY KEY (`filename`)
  ) ;
