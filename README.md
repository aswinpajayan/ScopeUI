# ScopeUI
small gtk + app to plot data recieved over wifi

# Abstract 
  The project aims at creating wireless DSO module. Which will sample and the analog voltage levels with a TivaC development board 
  and send the data over a wifi link using CC3100 addon board . This particular repository contains the code for the reception part. All the details of the project (both reciever and sender side ) is given in [Project Report](report/ee712_project_report.pdf)
  
  the code is written entirely in C. 
      ![Please read report/ee712_project_report](report/libraries_used.png)
     
 
      
The program is multithreaded to address any dead lock issue. Cairo library is used for drawing ui 
![Please read report/ee712_project_report](report/ui_snap.png)

# Dependencies 
	
	GTK3			  sudo apt-get install libgtk-3-dev
	
	glib			  sudo apt-get install libglib2.0-0 
	
	cairo			  sudo apt-get install libcairo2
	
	for all the build tools   sudo apt-get install build-essentials


# Building and running the project 
	
	use the make file provided in the project root
	
	
	make
	
	for building the project

	make run
	
	to launch the ui 

	click on the connect button . now the ui can start recieving UDP packets 
	
	You will have to get the ip address of the machine and update it on the sender 

### code for Tiva board is available at [wifi-DSO-module project by aswinajayan](https://github.com/aswinpajayan/wifi-DSO-module)

### Project members 
	1. Ananda Kundu 
	2. Sunny Mehtha 
	3. Aswin P Ajayan 
