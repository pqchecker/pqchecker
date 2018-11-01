#### Password strength checker for OpenLDAP password policy overlay

Allows to control the passwords quality (passwords strength) **before** storing them into the OpenLDAP directory server.  
If the password matches **configured** settings, it's accepted. Otherwise, it's rejected.  
The controlled parameters are:  
+ Number of required uppercase characters.
+ Number of required lowercase characters.
+ Number of required special characters.
+ Number of required digits.
+ List of forbidden characters.

The password strength settings are stored into a configuration text file who may be modified by a system administrator. But pqChecker allows reading and modifying these settings, programmatically. It allows, also, broadcasting the modified passwords, in real time, to another systems who use it (database system, mails server..). Broadcasting new / modified passwords may be activated / deacivated throug configuration settings and require uses of pqMessenger middleware when activated. see the pqmessenger project for further details.

##### pqChecker is part of a system composed by two independents modules

###### 1. Checking passwords module: pqchecker.so  (this project)
Native shared library for POSIX compliant systems. Checks modified passwords, before storing them into directory.

###### 2. Communicating with JMS compliant server: pqmessenger.jar 
Java daemon:
+ Allows reading and modifying passwords quality settings programmatically.
+ Allows broadcasting the modified passwords in real time.

![alt tag](http://www.meddeb.net/pqchecker/res/pqchecker-overview.png)

For further details visit http://www.meddeb.net/pqchecker
