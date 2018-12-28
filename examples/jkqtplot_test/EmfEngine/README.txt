EmfEngine GNU GPL v. 3.0 
------------------------
AUTHOR: Ion Vasilief
------------------------
FEATURES:  EmfEngine enables Qt based applications to easily export graphics created using the 
	   QPainter class to the Enhanced MetaFile format (EMF).
---------------------------------------------------------------------------
DEPENDENCIES: You need Qt (http://www.qtsoftware.com) installed on your system in order to build EmfEngine.
	      On Unix-like systems you also need libEMF (http://libemf.sourceforge.net/).
---------------------------------------------------------------------------
COMPILING: EmfEngine uses qmake for the building process. 
	qmake is part of a Qt distribution: 
	qmake reads project files, that contain the options and rules how to build a certain project. 
	A project file ends with the suffix "*.pro". Please read the qmake documentation for more details.

After installing Qt on your system, type the following command lines: 

	$ qmake
	$ make

if you use MinGW, or:

	$ qmake -tp vc -r

if you use Microsoft Visual Studio (this will generate Visual Studio .vcproj project files in the "src" and "example" folders).
---------------------------------------------------------------------------
USE: a short demo application is provided in the "example" folder of the source archive.
---------------------------------------------------------------------------
