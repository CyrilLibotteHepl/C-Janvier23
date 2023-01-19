.SILENT:

LibModele=modele/
LibVoiture=voiture/
LibOption=option/
LibPersonne=personne/
LibIntervenant=intervenant/
LibClient=client/
LibEmploye=employe/
LibException=exception/
LibOptionException=optionexception/
LibPasswordException=passwordexception/
LibVecteur=vecteur/
LibIterateur=iterateur/
LibVecteurTrie=vecteurtrie/
LibGarage=garage/
LibContrat=contrat/
DEBUG =# -D DEBUG
COMP=g++ -I $(LibModele) -I $(LibVoiture) -I $(LibOption) -I $(LibPersonne) -I $(LibIntervenant) -I $(LibClient) -I $(LibEmploye) -I $(LibException) -I $(LibOptionException) -I $(LibPasswordException) -I $(LibVecteur) -I $(LibIterateur) -I $(LibVecteurTrie) -I $(LibGarage) -I $(LibContrat)
OBJS=$(LibModele)/Modele.o $(LibVoiture)/Voiture.o $(LibOption)/Option.o $(LibPersonne)/Personne.o $(LibIntervenant)/Intervenant.o $(LibClient)/Client.o $(LibEmploye)/Employe.o $(LibException)/Exception.o $(LibOptionException)/OptionException.o $(LibPasswordException)/PasswordException.o $(LibVecteur)/Vecteur.o $(LibIterateur)/Iterateur.o $(LibVecteurTrie)/VecteurTrie.o $(LibGarage)/Garage.o $(LibContrat)/Contrat.o

CO=g++
OBJS_QT = main.o applicgaragewindow.o moc_applicgaragewindow.o
PROGRAMS = ApplicGarage 

all:	$(PROGRAMS)

ApplicGarage:	$(OBJS_QT) $(OBJS)
		echo Creation de ApplicGarage...
		$(COMP) -Wl,-O1 -o ApplicGarage $(OBJS_QT) $(OBJS) /usr/lib64/libQt5Widgets.so /usr/lib64/libQt5Gui.so /usr/lib64/libQt5Core.so /usr/lib64/libGL.so -lpthread 					
		#rm main.o

main.o:	main.cpp
		echo Creation de main.o
		$(COMP) -c -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o main.o main.cpp

applicgaragewindow.o:	applicgaragewindow.cpp applicgaragewindow.h ui_applicgaragewindow.h
		echo Creation de applicgaragewindow.o
		$(COMP) -c -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o applicgaragewindow.o applicgaragewindow.cpp

moc_applicgaragewindow.o:	moc_applicgaragewindow.cpp applicgaragewindow.h
		echo Creation de moc_applicgaragewindow.o
		$(COMP) -c -pipe -O2 -std=gnu++11 -Wall -W -D_REENTRANT -fPIC -DQT_DEPRECATED_WARNINGS -DQT_NO_DEBUG -DQT_WIDGETS_LIB -DQT_GUI_LIB -DQT_CORE_LIB -I. -isystem /usr/include/qt5 -isystem /usr/include/qt5/QtWidgets -isystem /usr/include/qt5/QtGui -isystem /usr/include/qt5/QtCore -I. -I. -I/usr/lib64/qt5/mkspecs/linux-g++ -o moc_applicgaragewindow.o moc_applicgaragewindow.cpp

$(LibModele)/Modele.o: $(LibModele)/Modele.cpp $(LibModele)/Modele.h
	echo Modele.o 
	$(COMP) $(LibModele)/Modele.cpp $(DEBUG) -c -o $(LibModele)/Modele.o 

$(LibVoiture)/Voiture.o: $(LibVoiture)/Voiture.cpp $(LibVoiture)/Voiture.h
	echo Voiture.o 
	$(COMP) $(LibVoiture)/Voiture.cpp $(DEBUG) -c -o $(LibVoiture)/Voiture.o

$(LibOption)/Option.o: $(LibOption)/Option.cpp $(LibOption)/Option.h
	echo Option.o 
	$(COMP) $(LibOption)/Option.cpp $(DEBUG) -c -o $(LibOption)/Option.o 

|$(LibPersonne)/Personne.o: $(LibPersonne)/Personne.cpp $(LibPersonne)/Personne.h
	echo Personne.o 
	$(COMP) $(LibPersonne)/Personne.cpp $(DEBUG) -c -o $(LibPersonne)/Personne.o

$(LibIntervenant)/Intervenant.o : $(LibIntervenant)/Intervenant.cpp $(LibIntervenant)/Intervenant.h $(LibPersonne)/Personne.o
	echo Intervenant.o 
	$(COMP) $(LibIntervenant)/Intervenant.cpp $(DEBUG) -c -o $(LibIntervenant)/Intervenant.o

$(LibClient)/Client.o : $(LibClient)/Client.cpp $(LibClient)/Client.h
	echo Client.o 
	$(COMP) $(LibClient)/Client.cpp $(DEBUG) -c -o $(LibClient)/Client.o  

$(LibEmploye)/Employe.o : $(LibEmploye)/Employe.cpp $(LibEmploye)/Employe.h $(LibIntervenant)/Intervenant.o
	echo Employe.o 
	$(COMP) $(LibEmploye)/Employe.cpp $(DEBUG) -c -o $(LibEmploye)/Employe.o

$(LibException)/Exception.o : $(LibException)/Exception.cpp $(LibException)/Exception.h $(LibIntervenant)/Intervenant.o
	echo Exception.o 
	$(COMP) $(LibException)/Exception.cpp $(DEBUG) -c -o $(LibException)/Exception.o

$(LibOptionException)/OptionException.o : $(LibOptionException)/OptionException.cpp $(LibOptionException)/OptionException.h $(LibException)/Exception.o
	echo OptionException.o 
	$(COMP) $(LibOptionException)/OptionException.cpp $(DEBUG) -c -o $(LibOptionException)/OptionException.o

$(LibPasswordException)/PasswordException.o : $(LibPasswordException)/PasswordException.cpp $(LibPasswordException)/PasswordException.h $(LibException)/Exception.o
	echo PasswordException.o 
	$(COMP) $(LibPasswordException)/PasswordException.cpp $(DEBUG) -c -o $(LibPasswordException)/PasswordException.o

$(LibVecteur)/Vecteur.o : $(LibVecteur)/Vecteur.cpp $(LibVecteur)/Vecteur.h
	echo Vecteur.o 
	$(COMP) $(LibVecteur)/Vecteur.cpp $(DEBUG) -c -o $(LibVecteur)/Vecteur.o

$(LibIterateur)/Iterateur.o : $(LibIterateur)/Iterateur.cpp $(LibIterateur)/Iterateur.h
	echo Iterateur.o 
	$(COMP) $(LibIterateur)/Iterateur.cpp $(DEBUG) -c -o $(LibIterateur)/Iterateur.o

$(LibVecteurTrie)/VecteurTrie.o : $(LibVecteurTrie)/VecteurTrie.cpp $(LibVecteurTrie)/VecteurTrie.h $(LibVecteur)/Vecteur.o
	echo VecteurTrie.o 
	$(COMP) $(LibVecteurTrie)/VecteurTrie.cpp $(DEBUG) -c -o $(LibVecteurTrie)/VecteurTrie.o

$(LibGarage)/Garage.o : $(LibGarage)/Garage.cpp $(LibGarage)/Garage.h 
	echo Garage.o 
	$(COMP) $(LibGarage)/Garage.cpp $(DEBUG) -c -o $(LibGarage)/Garage.o

$(LibContrat)/Contrat.o : $(LibContrat)/Contrat.cpp $(LibContrat)/Contrat.h 
	echo Contrat.o 
	$(COMP) $(LibContrat)/Contrat.cpp $(DEBUG) -c -o $(LibContrat)/Contrat.o


clean:
	@rm -f $(OBJS_QT) $(OBJS) core

clobber:	clean
	@rm -f tags $(PROGRAMS) *~ *.log
