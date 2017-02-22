*last update 22 Feb 2017*
# ERF

- OS username/passwd : oneadmin/oneadmin
- MySQL username/passwd : oneadmin/oneadmin
- Start script : /home/oneadmin/one5.2.0/start
- ReStart script : /home/oneadmin/one5.2.0/restart
- Stop script : /home/oneadmin/one5.2.0/stop
- Rebuild script : /home/oneadmin/one5.2.0/rebuild
- Sunstone : http://localhost:9869/
- Sunstone credential : oneadmin/oneadmin

# OpenNebula

Original guide
https://github.com/OpenNebula/one/blob/master/README.md

# Implementation 

This version is slightly different from the design due to 2 reasons
- First version is a prove the concept and We can achive some tasks with simple implementation (for example we run everything in same host).
- First version is first check point and need feedback from Advisor.

### ERF Log type
Add two new log types into OpenNebula , ERF and ERFTS (ERF Thread safe)
OpenNebula parse and create ERF and ERFTS log type object
> ~/one5.2.0/src/include/NebulaLog.h

ERFFileLog and ERFFileLogTS class implementation
> ~/one5.2.0/src/include/Log.h
> ~/one5.2.0/src/log/Log.cc

### Default Log type
Change default Log type to *ERSTS* in ~/one5.2.0/share/etc/oned.conf.
So, when we install OpenNebula, we will instancely use ERFLog type (/etc/one/oned.conf)

### Immediate Log Authentication (ILA)
ERFFileLog won't write log message to log file as usual. But it pass log message to ILA class for hash calculation. ILA class is also responsible for log rotation.
> ~/one5.2.0/src/include/ILA.h
> ~/one5.2.0/src/erf/ILA.cc

### ERF secret storage
We store secret in new table
>		CREATE TABLE `ERF` (
>		  `filename` varchar(100) NOT NULL,
>		  `hash` varchar(65) DEFAULT NULL,
>		  `lastupdate` datetime DEFAULT NULL,
>		  PRIMARY KEY (`filename`)
>		) ;

### Evidence Retrieval Service (ERS)
A separate process that receive evidence reqeust from ERF Client.
Currently ERS process and OpenNebula server run in same host.
> ~/one5.2.0/src/include/ERS.h
> ~/one5.2.0/src/erf/ERS.cc

Service file
> ~/one5.2.0/share/pkgs/CentOS7/ers.service

### ERF Client application java project
- ~/one5.2.0/src/erfclient
