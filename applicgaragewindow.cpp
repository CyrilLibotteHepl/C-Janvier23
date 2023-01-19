#include "applicgaragewindow.h"
#include "ui_applicgaragewindow.h"
#include <QMessageBox>
#include <QInputDialog>
#include <sstream>
#include <setjmp.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

#define REPERTOIRE_IMAGES "images/"

ApplicGarageWindow::ApplicGarageWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::ApplicGarageWindow)
{
    ui->setupUi(this);

    // Configuration de la table des options (projet en cours)
    ui->tableWidgetOptions->setColumnCount(3);
    ui->tableWidgetOptions->setRowCount(5);
    for (int i=0 ; i<5 ; i++) ui->tableWidgetOptions->setRowHeight(i,10);
    QStringList labelsTableOptions;
    labelsTableOptions << "Code" << "Prix" << "Intitulé";
    ui->tableWidgetOptions->setHorizontalHeaderLabels(labelsTableOptions);
    ui->tableWidgetOptions->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetOptions->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetOptions->horizontalHeader()->setVisible(true);
    ui->tableWidgetOptions->horizontalHeader()->setDefaultSectionSize(60);
    ui->tableWidgetOptions->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetOptions->verticalHeader()->setVisible(false);
    ui->tableWidgetOptions->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    // Configuration de la table des employes (Personnel Garage)
    ui->tableWidgetEmployes->setColumnCount(4);
    ui->tableWidgetEmployes->setRowCount(0);
    QStringList labelsTableEmployes;
    labelsTableEmployes << "Numéro" << "Nom" << "Prénom" << "Fonction";
    ui->tableWidgetEmployes->setHorizontalHeaderLabels(labelsTableEmployes);
    ui->tableWidgetEmployes->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetEmployes->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetEmployes->horizontalHeader()->setVisible(true);
    ui->tableWidgetEmployes->horizontalHeader()->setDefaultSectionSize(80);
    ui->tableWidgetEmployes->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetEmployes->verticalHeader()->setVisible(false);
    ui->tableWidgetEmployes->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    // Configuration de la table des clients
    ui->tableWidgetClients->setColumnCount(4);
    ui->tableWidgetClients->setRowCount(0);
    QStringList labelsTableClients;
    labelsTableClients << "Numéro" << "Nom" << "Prénom" << "GSM";
    ui->tableWidgetClients->setHorizontalHeaderLabels(labelsTableClients);
    ui->tableWidgetClients->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetClients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetClients->horizontalHeader()->setVisible(true);
    ui->tableWidgetClients->horizontalHeader()->setDefaultSectionSize(80);
    ui->tableWidgetClients->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetClients->verticalHeader()->setVisible(false);
    ui->tableWidgetClients->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    // Configuration de la table des contrats
    ui->tableWidgetContrats->setColumnCount(4);
    ui->tableWidgetContrats->setRowCount(0);
    QStringList labelsTableContrats;
    labelsTableContrats << "Numéro" << "Vendeur" << "Client" << "Voiture";
    ui->tableWidgetContrats->setHorizontalHeaderLabels(labelsTableContrats);
    ui->tableWidgetContrats->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetContrats->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetContrats->horizontalHeader()->setVisible(true);
    ui->tableWidgetContrats->horizontalHeader()->setDefaultSectionSize(80);
    ui->tableWidgetContrats->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetContrats->verticalHeader()->setVisible(false);
    ui->tableWidgetContrats->horizontalHeader()->setStyleSheet("background-color: lightyellow");

    // Importation des modeles (étape 10)
    // TO DO
    try
    {
        string nomFichier= "FichiersCSV/Modeles.csv";
        Garage::getInstance().importeModeles(nomFichier);

        Iterateur<Modele> itmod(Garage::getInstance().getIterateurModele());
        for(itmod.reset(); !itmod.end(); itmod++)
        {
            Modele mo = (Modele)itmod;
            ajouteModeleDisponible(mo.getNom(), mo.getPrixDeBase());
        }
    }
    catch (Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur Fichier", message.c_str());
    }

    // Importation des options (étape 10)
    // TO DO
    try
    {
        string nomFichier= "FichiersCSV/Options.csv";
        Garage::getInstance().importeOptions(nomFichier);

        Iterateur<Option> itop(Garage::getInstance().getIterateurOption());
        for(itop.reset(); !itop.end(); itop++)
        {
            Option op = (Option)itop;
            ajouteOptionDisponible(op.getIntitule(), op.getPrix());
        }
    }
    catch(Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur Fichier", message.c_str());
    }

    // Ouverture (et/ou creation) du fichier Garage.data (étape 12)
    // TO DO
    Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
    string nomf = "Garage.data";
    ifstream fichier(nomf, ios::in);

    if(!fichier)
    {
        ofstream fichier0(nomf, ios::out);
        string mdp = "123abc", nom = "Admin", prenom = "Admin" , login = "Admin" , fonction = "Administratif";
        Garage::getInstance().ajouteEmploye(nom, prenom, login, fonction);
        for(itemp.reset();!itemp.end() && login != ((Employe)itemp).getLogin();itemp++) {}
        
        Garage::getInstance().p = itemp.getAdresseCourante();
        Garage::getInstance().p->setMotDePasse(mdp);
        Garage::getInstance().p = NULL;

        Garage::getInstance().Save(fichier0);
    }
    else
    {
        Garage::getInstance().Load(fichier);
    }

    videTableEmployes();
    for (itemp.reset();!itemp.end();itemp++)
    {
        Employe emp = (Employe)itemp;
        if (emp.getLogin() != "Admin") ajouteTupleTableEmployes(emp.Tuple());
    }

    videTableClients();
    Iterateur <Client> itclt(Garage::getInstance().getIterateurClient());
    for (itclt.reset();!itclt.end();itclt++)
    {
        Client c = (Client)itclt;
        ajouteTupleTableClients(c.Tuple());
    }

    videTableContrats();
    Iterateur<Contrat> itctr(Garage::getInstance().getIterateurContrat());
    for(itctr.reset();!itctr.end();itctr++)
    {
        Contrat ct = (Contrat)itctr;
        ajouteTupleTableContrats(ct.Tuple());
    }

    //setModele("Peugeot 308",120,1,22300.0,"308.jpg");
    setRole(0);
}

ApplicGarageWindow::~ApplicGarageWindow()
{
    delete ui;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Titre de la fenêtre (ne pas modifier) //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::setTitre(string titre)
{
    this->setWindowTitle(titre.c_str());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Gestion des roles (ne pas modifier) ////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::setRole(int val)
{
    // val = 0 --> personne n'est loggé
    // val = 1 --> un administratif est loggé
    // val = 2 --> un vendeur loggé
    // val = 3 --> on a accès à tout

    if (val == 0)
    {
        ui->actionLogin->setEnabled(true);
        ui->actionLogout->setEnabled(false);
        ui->actionReset_Mot_de_passe->setEnabled(false);
        ui->menuEmployes->setEnabled(false);
        ui->menuClients->setEnabled(false);
        ui->menuVoiture->setEnabled(false);
        ui->comboBoxModelesDisponibles->setEnabled(false);
        ui->comboBoxOptionsDisponibles->setEnabled(false);
        ui->pushButtonOuvrirProjet->setEnabled(false);
        ui->pushButtonNouveauProjet->setEnabled(false);
        ui->pushButtonReduction->setEnabled(false);
        ui->pushButtonEnregistrerProjet->setEnabled(false);
        ui->pushButtonAjouterOption->setEnabled(false);
        ui->pushButtonSupprimerOption->setEnabled(false);
        ui->pushButtonChoisirModele->setEnabled(false);
        ui->pushButtonVisualiserVoiture->setEnabled(false);
        ui->pushButtonNouveauContrat->setEnabled(false);
        ui->pushButtonSupprimerContrat->setEnabled(false);
    }

    if (val == 1)
    {
        ui->actionLogin->setEnabled(false);
        ui->actionLogout->setEnabled(true);
        ui->actionReset_Mot_de_passe->setEnabled(true);
        ui->menuEmployes->setEnabled(true);
        ui->menuClients->setEnabled(false);
        ui->menuVoiture->setEnabled(false);
        ui->comboBoxModelesDisponibles->setEnabled(false);
        ui->comboBoxOptionsDisponibles->setEnabled(false);
        ui->pushButtonOuvrirProjet->setEnabled(false);
        ui->pushButtonNouveauProjet->setEnabled(false);
        ui->pushButtonReduction->setEnabled(false);
        ui->pushButtonEnregistrerProjet->setEnabled(false);
        ui->pushButtonAjouterOption->setEnabled(false);
        ui->pushButtonSupprimerOption->setEnabled(false);
        ui->pushButtonChoisirModele->setEnabled(false);
        ui->pushButtonVisualiserVoiture->setEnabled(true);
        ui->pushButtonNouveauContrat->setEnabled(false);
        ui->pushButtonSupprimerContrat->setEnabled(true);
    }

    if (val == 2)
    {
        ui->actionLogin->setEnabled(false);
        ui->actionLogout->setEnabled(true);
        ui->actionReset_Mot_de_passe->setEnabled(true);
        ui->menuEmployes->setEnabled(false);
        ui->menuClients->setEnabled(true);
        ui->menuVoiture->setEnabled(true);
        ui->comboBoxModelesDisponibles->setEnabled(true);
        ui->comboBoxOptionsDisponibles->setEnabled(true);
        ui->pushButtonOuvrirProjet->setEnabled(true);
        ui->pushButtonNouveauProjet->setEnabled(true);
        ui->pushButtonReduction->setEnabled(true);
        ui->pushButtonEnregistrerProjet->setEnabled(true);
        ui->pushButtonAjouterOption->setEnabled(true);
        ui->pushButtonSupprimerOption->setEnabled(true);
        ui->pushButtonChoisirModele->setEnabled(true);
        ui->pushButtonVisualiserVoiture->setEnabled(true);
        ui->pushButtonNouveauContrat->setEnabled(true);
        ui->pushButtonSupprimerContrat->setEnabled(true);
    }

    if (val == 3)
    {
        ui->actionLogin->setEnabled(true);
        ui->actionLogout->setEnabled(true);
        ui->actionReset_Mot_de_passe->setEnabled(true);
        ui->menuEmployes->setEnabled(true);
        ui->menuClients->setEnabled(true);
        ui->menuVoiture->setEnabled(true);
        ui->comboBoxModelesDisponibles->setEnabled(true);
        ui->comboBoxOptionsDisponibles->setEnabled(true);
        ui->pushButtonOuvrirProjet->setEnabled(true);
        ui->pushButtonNouveauProjet->setEnabled(true);
        ui->pushButtonReduction->setEnabled(true);
        ui->pushButtonEnregistrerProjet->setEnabled(true);
        ui->pushButtonAjouterOption->setEnabled(true);
        ui->pushButtonSupprimerOption->setEnabled(true);
        ui->pushButtonChoisirModele->setEnabled(true);
        ui->pushButtonVisualiserVoiture->setEnabled(true);
        ui->pushButtonNouveauContrat->setEnabled(true);
        ui->pushButtonSupprimerContrat->setEnabled(true);
    }

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Table des options du projet en cours (ne pas modifier) /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::setTableOption(int indice,string code,string intitule,float prix)
{
    // Modifie l'option de la ligne correspondant à l'indice fourni dans la table des options de l'interface graphique
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(code.c_str());
    ui->tableWidgetOptions->setItem(indice,0,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    char tmp[20];
    sprintf(tmp,"%.2f",prix);
    if (prix < 0.0) item->setText("");
    else item->setText(tmp);
    ui->tableWidgetOptions->setItem(indice,1,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(intitule.c_str());
    ui->tableWidgetOptions->setItem(indice,2,item);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::videTableOption()
{
    for (int i=0 ; i<5 ; i++) setTableOption(i);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::getIndiceOptionSelectionneeTable() const
{
    // retourne l'indice de l'option selectionnee dans la table des options
    QModelIndexList liste = ui->tableWidgetOptions->selectionModel()->selectedRows();
    if (liste.size() == 0) return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Combobox des modèles disponibles (ne pas modifier) /////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::ajouteModeleDisponible(const char* nom,float prixDeBase)
{
    // ajoute un modèle dans le combobox correspondant (juste le nom et le prix)
    stringstream ss;
    char temp[20];
    sprintf(temp,"%.2f",prixDeBase);
    ss << nom << " (" << temp << ")";
    ui->comboBoxModelesDisponibles->addItem(ss.str().c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::videComboboxModelesDisponibles()
{
    ui->comboBoxModelesDisponibles->clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::getIndiceModeleSelectionneCombobox() const
{
    // retourne l'indice du modèle selectionné dans le combobox des modèles
    return ui->comboBoxModelesDisponibles->currentIndex();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Combox des options disponibles (ne pas modifier) ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::ajouteOptionDisponible(string intitule,float prix)
{
    // ajoute une option dans le combobox correspondant
    stringstream ss;
    char temp[20];
    sprintf(temp,"%.2f",prix);
    ss << intitule << " (" << temp << ")";
    ui->comboBoxOptionsDisponibles->addItem(ss.str().c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::videComboboxOptionsDisponibles()
{
    ui->comboBoxOptionsDisponibles->clear();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::getIndiceOptionSelectionneeCombobox() const
{
    // retourne l'indice de l'option selectionnée dans le combobox des options
    return ui->comboBoxOptionsDisponibles->currentIndex();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Gestion du Projet en cours (ne pas modifier) ///////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::setNomProjetEnCours(string nom)
{
    // Remplit le champ nom de l'interface graphique avec le nom reçu
    ui->lineEditNomProjet->setText(nom.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string ApplicGarageWindow::getNomProjetEnCours() const
{
    return ui->lineEditNomProjet->text().toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::setModele(string nom,int puissance,int moteur,float prixDeBase,string imageModele)
{
    // Remplit les champs correspondants dans l'interface graphique
    // pour le moteur (int) : 0=Essence, 1=Diesel, 2=Electrique, 3=Hybride
    ui->lineEditNomModele->setText(nom.c_str());
    ui->lineEditPuissance->setText(to_string(puissance).c_str());
    char tmp[20];
    sprintf(tmp,"%.2f",prixDeBase);
    ui->lineEditPrixDeBase->setText(tmp);

    ui->radioButtonEssence->setChecked(false);
    ui->radioButtonDiesel->setChecked(false);
    ui->radioButtonElectrique->setChecked(false);
    ui->radioButtonHybride->setChecked(false);

    if (moteur == 0) ui->radioButtonEssence->setChecked(true);
    if (moteur == 1) ui->radioButtonDiesel->setChecked(true);
    if (moteur == 2) ui->radioButtonElectrique->setChecked(true);
    if (moteur == 3) ui->radioButtonHybride->setChecked(true);

    // Met à jour l'image du modèle
    string cheminComplet = REPERTOIRE_IMAGES + imageModele;
    QLabel* label = new QLabel();
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    label->setScaledContents(true);
    QPixmap *pixmap_img = new QPixmap(cheminComplet.c_str());
    label->setPixmap(*pixmap_img);
    label->resize(label->pixmap()->size());
    ui->scrollArea->setWidget(label);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::setPrix(float prix)
{
  char tmp[20];
  sprintf(tmp,"%.2f",prix);
  ui->lineEditPrix->setText(tmp);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des Employes (ne pas modifier) //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::ajouteTupleTableEmployes(string tuple)
{
    // Controle du tuple recu et parsing
    char Tuple[200];
    strncpy(Tuple,tuple.c_str(),199);
    char* tmp;

    if ((tmp = strtok(Tuple,";")) == NULL) return;
    char numero[20];
    strcpy(numero,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char nom[40];
    strcpy(nom,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char prenom[40];
    strcpy(prenom,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char fonction[40];
    strcpy(fonction,tmp);

    // Ajout possible
    int nbLignes = ui->tableWidgetEmployes->rowCount();
    nbLignes++;
    ui->tableWidgetEmployes->setRowCount(nbLignes);
    ui->tableWidgetEmployes->setRowHeight(nbLignes-1,10);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(numero);
    ui->tableWidgetEmployes->setItem(nbLignes-1,0,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(nom);
    ui->tableWidgetEmployes->setItem(nbLignes-1,1,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(prenom);
    ui->tableWidgetEmployes->setItem(nbLignes-1,2,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(fonction);
    ui->tableWidgetEmployes->setItem(nbLignes-1,3,item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::videTableEmployes()
{
    ui->tableWidgetEmployes->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::getIndiceEmployeSelectionne()
{
    QModelIndexList liste = ui->tableWidgetEmployes->selectionModel()->selectedRows();
    if (liste.size() == 0) return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des Clients (ne pas modifier) ///////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::ajouteTupleTableClients(string tuple)
{
    // Controle du tuple recu et parsing
    char Tuple[200];
    strncpy(Tuple,tuple.c_str(),199);
    char* tmp;

    if ((tmp = strtok(Tuple,";")) == NULL) return;
    char numero[20];
    strcpy(numero,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char nom[40];
    strcpy(nom,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char prenom[40];
    strcpy(prenom,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char gsm[40];
    strcpy(gsm,tmp);

    // Ajout possible
    int nbLignes = ui->tableWidgetClients->rowCount();
    nbLignes++;
    ui->tableWidgetClients->setRowCount(nbLignes);
    ui->tableWidgetClients->setRowHeight(nbLignes-1,10);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(numero);
    ui->tableWidgetClients->setItem(nbLignes-1,0,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(nom);
    ui->tableWidgetClients->setItem(nbLignes-1,1,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(prenom);
    ui->tableWidgetClients->setItem(nbLignes-1,2,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(gsm);
    ui->tableWidgetClients->setItem(nbLignes-1,3,item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::videTableClients()
{
    ui->tableWidgetClients->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::getIndiceClientSelectionne()
{
    QModelIndexList liste = ui->tableWidgetClients->selectionModel()->selectedRows();
    if (liste.size() == 0) return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions utiles Table des Contrats (ne pas modifier) //////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::ajouteTupleTableContrats(string tuple)
{
    // Controle du tuple recu et parsing
    char Tuple[200];
    strncpy(Tuple,tuple.c_str(),199);
    char* tmp;

    if ((tmp = strtok(Tuple,";")) == NULL) return;
    char numero[20];
    strcpy(numero,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char vendeur[40];
    strcpy(vendeur,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char client[40];
    strcpy(client,tmp);

    if ((tmp = strtok(NULL,";")) == NULL) return;
    char voiture[40];
    strcpy(voiture,tmp);

    // Ajout possible
    int nbLignes = ui->tableWidgetContrats->rowCount();
    nbLignes++;
    ui->tableWidgetContrats->setRowCount(nbLignes);
    ui->tableWidgetContrats->setRowHeight(nbLignes-1,10);

    QTableWidgetItem *item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setTextAlignment(Qt::AlignCenter);
    item->setText(numero);
    ui->tableWidgetContrats->setItem(nbLignes-1,0,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(vendeur);
    ui->tableWidgetContrats->setItem(nbLignes-1,1,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(client);
    ui->tableWidgetContrats->setItem(nbLignes-1,2,item);

    item = new QTableWidgetItem;
    item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    item->setText(voiture);
    ui->tableWidgetContrats->setItem(nbLignes-1,3,item);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::videTableContrats()
{
    ui->tableWidgetContrats->setRowCount(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::getIndiceContratSelectionne()
{
    QModelIndexList liste = ui->tableWidgetContrats->selectionModel()->selectedRows();
    if (liste.size() == 0) return -1;
    QModelIndex index = liste.at(0);
    int indice = index.row();
    return indice;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions permettant d'afficher des boites de dialogue /////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::dialogueMessage(const char* titre,const char* message)
{
   QMessageBox::information(this,titre,message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::dialogueErreur(const char* titre,const char* message)
{
   QMessageBox::critical(this,titre,message);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
string ApplicGarageWindow::dialogueDemandeTexte(const char* titre,const char* question)
{
    QString chaine = QInputDialog::getText(this,titre,question);
    return chaine.toStdString();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
int ApplicGarageWindow::dialogueDemandeInt(const char* titre,const char* question)
{
    return QInputDialog::getInt(this,titre,question);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
float ApplicGarageWindow::dialogueDemandeFloat(const char* titre,const char* question)
{
    return QInputDialog::getDouble(this,titre,question);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les items de menu //////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionQuitter_triggered()
{
  // TO DO (étape 12)
    string mdp;
    Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
    for(itemp.reset();!itemp.end();itemp++)
    {
        Garage::getInstance().p = itemp.getAdresseCourante();
        try
        {
            mdp = ((Employe)itemp).getMotDePasse();
        }
        catch (PasswordException pex)
        {
            string code = to_string(pex.getCode()); 
            dialogueErreur("Mot de passe", "Il ne peu y avoir de mot de passe indéterminé lors de la fermeture de l'application.");
            mdp = "Choisissez le mdp de " + ((Employe)itemp).getLogin();
            dialogueMessage("Mot de passe", mdp.c_str());
            on_actionReset_Mot_de_passe_triggered();
        }
    }
    ofstream fichier1("Garage.data", ios::out);
    Garage::getInstance().Save(fichier1);    
    if(!fichier1)
    {
        dialogueErreur("Erreur fichier", "Erreur de sauvegarde du fichier.");
        return;
    }
  QApplication::exit();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::closeEvent(QCloseEvent *event)
{
  if (event == NULL) {}  // pour éviter le warning
  // TO DO (étape 12)
    string mdp;
    Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
    for(itemp.reset();!itemp.end();itemp++)
    {
        Garage::getInstance().p = itemp.getAdresseCourante();
        try
        {
            mdp = ((Employe)itemp).getMotDePasse();
        }
        catch (PasswordException pex)
        {
            string code = to_string(pex.getCode()); 
            dialogueErreur("Mot de passe", "Il ne peu y avoir de mot de passe indéterminé lors de la fermeture de l'application.");
            mdp = "Choisissez le mdp de " + ((Employe)itemp).getLogin();
            dialogueMessage("Mot de passe", mdp.c_str());
            on_actionReset_Mot_de_passe_triggered();
        }
    }
    ofstream fichier2("Garage.data", ios::out);
    Garage::getInstance().Save(fichier2);  
    if(!fichier2)
    {
        dialogueErreur("Erreur fichier", "Erreur de sauvegarde du fichier.");
        return;
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionNouveau_modele_triggered()
{
  // Saisie des informations (aucune vérification n'est faite --> à gérer si vous voulez)
  // TO DO (étape 9)
  try
  {
    int faux = 0;
    Moteur mot = Essence;

    string nomModele = dialogueDemandeTexte("Nouveau modèle","Nom :");
    if (nomModele.size() == 0)
    {
      faux = 1;
    }
    else
    {
         //char *nM = new char[(nomModele.size())+1];
      Iterateur <Modele> itMod(Garage::getInstance().getIterateurModele());
      for (itMod.reset();!itMod.end();itMod++)
      {
        Modele mod = (Modele)itMod;
        string nomCourant = mod.getNom();
        if (nomModele == nomCourant)
            throw Exception ("Le modèle que vous encodé est déjà présent.");
        
      }
    }

    int puissance;
    puissance = dialogueDemandeInt("Nouveau modèle","Puissance :");
    if (puissance <= 0)
    {
        faux = 2;
    }

    int moteur;
    moteur = dialogueDemandeInt("Nouveau modèle :","Moteur (0=essence,1=diesel,2=électrique,3=hybride) :");
    if (moteur < 0 || moteur > 3)
    {
        faux = 3;
    }
    else
    {
        switch(moteur)
        {
            case 0: mot = Essence;
                break;
            case 1: mot = Diesel;
                break;
            case 2: mot = Electrique;
                break;
            case 3: mot = Hybride;
                break;
        }
                
    }
    
    float prixDeBase = 0;
    prixDeBase = dialogueDemandeFloat("Nouveau modèle","Prix de base :");
    if (prixDeBase < 0)
    {
        faux = 4;
    }
    string image;
    image = dialogueDemandeTexte("Nouveau modèle","Nom du fichier de l'image :");
    if (faux != 0)
    {
        switch(faux)
        {
            case 1: throw Exception ("Le nom ne peut être NULL");
                break;
            case 2: throw Exception ("La puissance ne peut être NULL");
                break;
            case 3: throw Exception ("Choisissez un nombre entre 0 et 3");
                break;
            case 4: throw Exception ("Le prix ne peut être négatif");
                break;
        }
    }
    
    
    Modele m(nomModele.c_str(), puissance, mot, prixDeBase, image);
    Garage::getInstance().ajouteModele(m);
    ajouteModeleDisponible(nomModele.c_str(), prixDeBase);
  }
  catch (Exception excp)
  {
    string message = excp.getMessage();
    // char *maf = new char[message.size()+1];
    // copy(message.begin(), message.end(), maf);
    // maf[message.size()+1] = '\0';
    dialogueErreur("Eureur modele", message.c_str());
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionNouvelle_option_triggered()
{
    // TO DO (étape 9)
    // Saisie des informations (aucune vérification n'est faite --> à gérer si vous voulez)
    try
    {
        string code = dialogueDemandeTexte("Nouvelle option","Code :");
        Iterateur <Option> itopt(Garage::getInstance().getIterateurOption());
        for (itopt.reset();!itopt.end();itopt++)
        {
            Option op = (Option)itopt;
            if (code == op.getCode())
            {
                throw Exception ("Le modèle que voue encodé est déjà présent.");
            }
        }
        string intitule = dialogueDemandeTexte("Nouvelle option","Intitule :");
        float prix = dialogueDemandeFloat("Nouvelle option","Prix :");

        Option op(code, intitule, prix);
        Garage::getInstance().ajouteOption(op);
        ajouteOptionDisponible(intitule, prix);

    }
    catch(OptionException m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur option", message.c_str());
    }
    catch(Exception excp)
    {
        string message = excp.getMessage();
        dialogueErreur("Eureur modele", message.c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionAjouterEmploye_triggered()
{
  // TO DO (étape 11)  
    try
    {
        string nom = dialogueDemandeTexte("Nouvelle employé","Nom :");
        if (nom.size() == 0)
        {
            throw Exception ("Le nom ne peu être null.");
        }
        string prenom = dialogueDemandeTexte("Nouvelle employé","Prenom : ");
        if (prenom.size() == 0)
        {
            throw Exception ("Le nom ne peu être null.");
        }
        string login = dialogueDemandeTexte("Nouvelle employé","Login :");
        if (login.size() == 0)
        {
            throw Exception ("Le login ne peu être null.");
        }

        int fonct, test = 0;
        Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
        for(itemp.reset();!itemp.end() && test == 0;itemp++)
        {
            Employe em = (Employe)itemp;
            if (login == em.getLogin()) test = 1;
        }

        if (test == 1)
            throw Exception ("Cette employé est déjà présent.");

        string fonction;  

        fonct = dialogueDemandeInt("Nouvelle employé","Fontion : 1.Administratif 2.Vendeur");
        switch(fonct)
        {
            case 1: fonction = "Administratif";
                    break;
            case 2: fonction = "Vendeur";
                    break;
            default: throw Exception ("Le nombre rentré pour la fonction ne correspond pas.");
        }

        string mdp = dialogueDemandeTexte("Nouvelle employé", "Entrez votre mot de passe : ");
        Employe emp;
        emp.setMotDePasse(mdp); //////test si le mdp est valide pour ne pas ajouté l'employé avec un mot de passe invalide

        Garage::getInstance().ajouteEmploye(nom, prenom, login, fonction);

        for(itemp.reset();!itemp.end() && login != ((Employe)itemp).getLogin();itemp++) {}
        Garage::getInstance().p = itemp.getAdresseCourante(); //modification du mdp avec le pointeur public de garage
        Garage::getInstance().p->setMotDePasse(mdp);

        Garage::getInstance().p = NULL;
        videTableEmployes();

        for (itemp.reset();!itemp.end();itemp++)
        {
            Employe emp = (Employe)itemp;
            if (emp.getLogin() != "Admin") ajouteTupleTableEmployes(emp.Tuple());
        }
    }
    catch(PasswordException pex)
    {
        string code = to_string(pex.getCode()); 
        string message = pex.getMessage();
        message = message + " Code : " + code;
        dialogueErreur("Eureur mot de passe", message.c_str());
    }
    catch(Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur employé", message.c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionSupprimerEmploye_par_numero_triggered()
{
  // TO DO (étape 11)
    try
    {
        int tmp = 0, numCourant = dialogueDemandeInt("Supprimer employé", "Entré le numéro de l'employé : ");
        Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
        for (itemp.reset();!itemp.end();itemp++)
        {
            Employe em = (Employe)itemp;
            if (numCourant == em.getNumero()) tmp = 1;
        }
        if (tmp == 0)
            throw Exception ("Le numéro entré ne correspond à aucun employé.");

        Garage::getInstance().supprimeEmployeParNumero(numCourant);
        videTableEmployes();

        for (itemp.reset();!itemp.end();itemp++)
        {
            Employe emp = (Employe)itemp;
            if (emp.getLogin() != "Admin") ajouteTupleTableEmployes(emp.Tuple());
        }
    }
    catch (Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur employé", message.c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionSupprimerEmploye_selection_triggered()
{
  // TO DO (étape 11)
    int ind;
    if ((ind = getIndiceEmployeSelectionne()) == -1)
    {
        dialogueErreur("Eureur de sélection", "Aucun employé n'est séléctionner");
        return;
    }
    
    Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
    Garage::getInstance().supprimeEmployeParIndice(ind);
    videTableEmployes();

    for (itemp.reset();!itemp.end();itemp++)
    {
        Employe emp = (Employe)itemp;
        if (emp.getLogin() != "Admin") ajouteTupleTableEmployes(emp.Tuple());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionAjouterClient_triggered()
{
  // TO DO (étape 11)
    try
    {
        string nom = dialogueDemandeTexte("Nouveau client","Nom :");
        if (nom.size() == 0)
        {
            throw Exception ("Le nom ne peu être null.");
        }
        string prenom = dialogueDemandeTexte("Nouveau client","Prenom : ");
        if (prenom.size() == 0)
        {
            throw Exception ("Le nom ne peu être null.");
        }
        string gsm = dialogueDemandeTexte("Nouveau client","Gsm :");
        if (gsm.size() == 0)
        {
            throw Exception ("Le gsm ne peu être null.");
        }

        int test = 0;
        Iterateur <Client> itclt(Garage::getInstance().getIterateurClient());
        for(itclt.reset();!itclt.end() && test == 0;itclt++)
        {
            Client c = (Client)itclt;
            if (gsm == c.getGsm()) test = 1;
        }

        if (test == 1)
            throw Exception ("Ce client est déjà présent.");


        Garage::getInstance().ajouteClient(nom, prenom, gsm);

        videTableClients();   //tu vide et te refait tout parce que énoncé et que tu ne connais pas l'indice de la première case vide

        for (itclt.reset();!itclt.end();itclt++)
        {
            Client c = (Client)itclt;
            ajouteTupleTableClients(c.Tuple());
        }
    }
    catch(Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur employé", message.c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionSupprimerClient_par_numero_triggered()
{
  // TO DO (étape 11)
    try
    {
        int tmp = 0, numCourant = dialogueDemandeInt("Supprimer client", "Entré le numéro du client : ");
        Iterateur <Client> itclt(Garage::getInstance().getIterateurClient());
        for (itclt.reset();!itclt.end();itclt++)
        {
            Client c = (Client)itclt;
            if (numCourant == c.getNumero()) tmp = 1;
        }
        if (tmp == 0)
            throw Exception ("Le numéro entré ne correspond à aucun client.");

        Garage::getInstance().supprimeClientParNumero(numCourant);
        videTableClients();

        for (itclt.reset();!itclt.end();itclt++)
        {
            Client c = (Client)itclt;
            ajouteTupleTableClients(c.Tuple());
        }
    }
    catch (Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur employé", message.c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionSupprimerClient_selection_triggered()
{
  // TO DO (étape 11)
    int ind;
    if ((ind = getIndiceClientSelectionne()) == -1)
    {
        dialogueErreur("Eureur de sélection", "Aucun client n'est séléctionner");
        return;
    }
    
    Iterateur <Client> itclt(Garage::getInstance().getIterateurClient());
    Garage::getInstance().supprimeClientParIndice(ind);
    videTableClients();

    for (itclt.reset();!itclt.end();itclt++)
    {
        Client c = (Client)itclt;
        ajouteTupleTableClients(c.Tuple());
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionLogin_triggered()
{
  // TO DO (étape 11)
    try
    {
        Garage::getInstance().p = NULL;
        string login = dialogueDemandeTexte("Login", "Entrez votre login : ");
        if (login.size() == 0)
        {
            throw Exception ("Le login ne peu être null.");
        }

        Iterateur <Employe> itemp(Garage::getInstance().getIterateurEmploye());
        int test = 0;
        for (itemp.reset();!itemp.end() && test == 0;itemp++)
        {
            Employe emp = (Employe)itemp;
            if (login == emp.getLogin()) 
            {
                Garage::getInstance().p = itemp.getAdresseCourante();
                test = 1;
            }
        }

        if (test == 1)
        {
            string mdp, mdp2 = (*Garage::getInstance().p).getMotDePasse();
            mdp = dialogueDemandeTexte("Mot de passe", "Entrez votre mot de passe : ");
            if(mdp == mdp2)
            {
                dialogueMessage("Login", "Entrée en session réussie");
                if((Garage::getInstance().p)->getFonction() == "Administratif")
                {
                    setRole(1);
                }
                else
                {
                    setRole(2);
                }
                setTitre((Garage::getInstance().p)->getLogin());
            }
            else
            {
                throw Exception ("Mot de passe incorect.");
            }
        }
        else
        {
            throw Exception ("Le login que vous avez encodé n'existe pas.");
        }
    }
    catch(PasswordException pex)
    {
        string code = to_string(pex.getCode()); 
        string message = pex.getMessage();
        message = message + " Code : " + code;
        dialogueErreur("Eureur Code", message.c_str()); //un core dupt est potentiel quand on reset le mot de passe à null
        if (code == "4") //pour éviter un core dupt avec le chargement d'une donnée null ou a une exception dans le get de employé qui nous permet qu'il
        {                 // n'arrive pas, ensuite si on souhaite ce connecté à cette employé il faut pouvoir lui créer un mdp
            string mdp = dialogueDemandeTexte("Mot de passe", "Créez votre mot de passe : ");
            Garage::getInstance().p->setMotDePasse(mdp);
            dialogueMessage("Login", "Entrée en session réussie");
            if((Garage::getInstance().p)->getFonction() == "Administratif")
            {
                setRole(1);
            }
            else
            {
                setRole(2);
            }
            setTitre((Garage::getInstance().p)->getLogin());
        }
    }
    catch (Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Erreur login", message.c_str());
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionLogout_triggered()
{
  // TO DO (étape 11)
    string vide = "";
    setTitre(vide);
    setRole(0);
    Garage::getInstance().p = NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_actionReset_Mot_de_passe_triggered()
{
  // TO DO (étape 11)
    try
    {
        Garage::getInstance().p->ResetMotDePasse();
        dialogueMessage("Mot de passe", "Le mot de passe à été reset.");
        string mdp = dialogueDemandeTexte("Mot de passe", "Nouveau mot de passe : ");
        Garage::getInstance().p->setMotDePasse(mdp);
        
    }
    catch(PasswordException pex)
    {
        string code = to_string(pex.getCode()); 
        string message = pex.getMessage();
        message = message + " Code : " + code;
        dialogueErreur("Eureur Code", message.c_str());
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///// Fonctions clics sur les boutons ////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonChoisirModele_clicked() 
{
  // TO DO (étape 9) 
    int ind, puissance, moteur = 0;
    string nom;
    float prixDeBase = 0; 
    string image;

    if ((ind = getIndiceModeleSelectionneCombobox()) == -1)
    {
        dialogueErreur("Eureur de sélection", "Aucun modéle n'est séléctionner");
        return;
    }

    string nompp = getNomProjetEnCours();
    if (nompp.size() == 0)
    {
        dialogueErreur("Eureur nom", "Le projet doit obligatoirement avoir un nom.");
        return;
    }

    Garage::getProjetEnCours().setModele(Garage::getInstance().getModele(ind));
    Modele m = Garage::getProjetEnCours().getModele();
    prixDeBase = m.getPrixDeBase();
    nom = m.getNom();
    puissance = m.getPuissance();
    Moteur mot = m.getMoteur();
    image = m.getImage();

    switch(mot)
    {
        case Essence: moteur = 0;
            break;
        case Diesel: moteur = 1;
            break;
        case Electrique: moteur = 2;
            break;
        case Hybride: moteur = 3;
            break;
    }

    ApplicGarageWindow::setModele(nom, puissance, moteur, prixDeBase, image);
    float prix = Garage::getProjetEnCours().getPrix();
    ApplicGarageWindow::setPrix(prix);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonAjouterOption_clicked()
{
  // TO DO (étape 9) 
    int ind;
    if ((ind = getIndiceOptionSelectionneeCombobox())== -1)
    {
        dialogueErreur("Eureur de sélection", "Aucune option n'est séléctionner");
        return;
    }

    try
    {
        Option op = Garage::getInstance().getOption(ind);

        int i = 0;
        for (i = 0; i<5 && Garage::getProjetEnCours()[i] != NULL; i++) {}

        Garage::getProjetEnCours().AjouteOption(op); //ici qu'on vérifie si il y trop d'option

        setTableOption(i,  op.getCode(),  op.getIntitule(),  op.getPrix());
        float prix = Garage::getProjetEnCours().getPrix();
        ApplicGarageWindow::setPrix(prix);
    }
    catch(OptionException m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur saisie", message.c_str());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonSupprimerOption_clicked()
{
  // TO DO (étape 9)
    int ind/*, i = 0*/;
    string  code  =  "";
    string  intitule  =  "";
    float prix   = -1.0;
    if((ind = getIndiceOptionSelectionneeTable()) == -1 || Garage::getProjetEnCours()[ind] == NULL) 
    {
        dialogueErreur("Eureur de sélection", "Aucune option n'est séléctionner");
        return;
    }
    Option* op = Garage::getProjetEnCours()[ind];

    Garage::getProjetEnCours().RetireOption(op->getCode());
    setTableOption(ind, code, intitule, prix);
    float prixTOT = Garage::getProjetEnCours().getPrix();
    ApplicGarageWindow::setPrix(prixTOT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonReduction_clicked() 
{
  // TO DO (étape 9) 
    try
    {
        int ind;
        string  code  =  "";
        string  intitule  =  "";
        float prix = 0;
        if((ind = getIndiceOptionSelectionneeTable()) == -1 || Garage::getProjetEnCours()[ind] == NULL)
        {
            dialogueErreur("Eureur de sélection", "Aucune option n'est séléctionner");
            return;
        }
        --*(Garage::getProjetEnCours()[ind]);
        code = Garage::getProjetEnCours()[ind]->getCode();
        intitule = Garage::getProjetEnCours()[ind]->getIntitule();
        prix = Garage::getProjetEnCours()[ind]->getPrix();
        setTableOption(ind, code, intitule, prix);
        float prixTOT = Garage::getProjetEnCours().getPrix();
        ApplicGarageWindow::setPrix(prixTOT);
    }
    catch(OptionException m)
    {
        string message = m.getMessage();
        dialogueErreur("Eureur saisie", message.c_str());
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonEnregistrerProjet_clicked()                                            
{
  // TO DO (étape 9)
    string nom = getNomProjetEnCours();
    if (nom.size() == 0)
    {
        dialogueErreur("Eureur nom", "Le projet doit obligatoirement avoir un nom.");
        nom = dialogueDemandeTexte("Nom fichier","Nom du fichier.");
        setNomProjetEnCours(nom);
    }
    Garage::getProjetEnCours().setNom(nom);
    Garage::getProjetEnCours().Save();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonOuvrirProjet_clicked()
{
  // TO DO (étape 9)
    ApplicGarageWindow::on_pushButtonNouveauProjet_clicked();
    string nom = dialogueDemandeTexte("Nom fichier","Nom du fichier à ouvrir (sans .car):");
    setNomProjetEnCours(nom);
    nom = nom + ".car";
    Garage::getProjetEnCours().Load(nom);
    Modele m = Garage::getProjetEnCours().getModele();
    string nomModele = m.getNom();
    Moteur moteur = m.getMoteur();
    int mot = 0;
    switch(moteur)
    {
        case Essence: mot = 0;
            break;
        case Diesel: mot = 1;
            break;
        case Electrique: mot = 2;
            break;
        case Hybride: mot = 3;
            break;
    }
    
    ApplicGarageWindow::setModele(nomModele.c_str(), m.getPuissance(), mot, m.getPrixDeBase(), (m.getImage()).c_str());
    ajouteModeleDisponible(nomModele.c_str(), m.getPrixDeBase());

    for(int i=0; i<5; i++)
    {
        if(Garage::getProjetEnCours()[i] != NULL)
        {
            Option op = *(Garage::getProjetEnCours()[i]);
            setTableOption(i, op.getCode(), op.getIntitule(), op.getPrix());
            ajouteOptionDisponible(op.getIntitule(), op.getPrix());
        }
    }

    ApplicGarageWindow::setPrix(Garage::getProjetEnCours().getPrix());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonNouveauProjet_clicked()
{
  // TO DO (étape 9)
    string confirmation = dialogueDemandeTexte ("Projet","Voulez-vous sauvegarder le projet en cours ? (Oui/Non)");
    if (confirmation != "Non" && confirmation != "non")
    {
        dialogueMessage("Chargement projet","Il vas être sauvegarder.");
        ApplicGarageWindow::on_pushButtonEnregistrerProjet_clicked();
    }

    ui->lineEditNomModele->clear();
    ui->lineEditPuissance->clear();
    ui->lineEditPrixDeBase->clear();
    ui->lineEditPrix->clear();
    ui->lineEditNomProjet->clear();

    ui->radioButtonEssence->setChecked(true);
    ui->radioButtonDiesel->setChecked(false);
    ui->radioButtonElectrique->setChecked(false);
    ui->radioButtonHybride->setChecked(false);

    // Met à jour l'image du modèle
    QLabel* label = new QLabel();
    ui->scrollArea->setWidget(label);

    for (int ind=0 ; ind < 5 ;ind++)
    {
        if (Garage::getProjetEnCours()[ind] != NULL)
        {
            Option* op = Garage::getProjetEnCours()[ind];
            Garage::getProjetEnCours().RetireOption(op->getCode());
        }
    }

    ApplicGarageWindow::videTableOption();


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonNouveauContrat_clicked()
{
  // TO DO (étape 13)
    try
    {
        string nompr = getNomProjetEnCours();
        Iterateur<Contrat> itctr(Garage::getInstance().getIterateurContrat());
        for (itctr.reset();!itctr.end() && ((Contrat)itctr).getNomProjet() != nompr; itctr++) {}

        if (!itctr.end())
        {
            throw Exception ("Deux contrat ne peuvent avoir le même nom.");
        }

        const Employe* pe = Garage::getInstance().p;
        on_pushButtonEnregistrerProjet_clicked();

        int ind;
        if ((ind = getIndiceClientSelectionne()) == -1)
        {
            dialogueErreur("Eureur de sélection", "Aucun client n'est séléctionner");
            return;
        }

        Iterateur<Client> itctl(Garage::getInstance().getIterateurClient());
        int i = 0;
        for(itctl.reset();!itctl.end() && ind != i;itctl++)
        {
            i++;
        }
        const Client * cl = itctl.getAdresseCourante();

        Garage::getInstance().ajouteContrat(nompr, pe, cl);

        videTableContrats();

        for(itctr.reset();!itctr.end();itctr++)
        {
            Contrat ct = (Contrat)itctr;
            ajouteTupleTableContrats(ct.Tuple());
        }
    }
    catch (Exception m)
    {
        string message = m.getMessage();
        dialogueErreur("Erreur login", message.c_str());
    }

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonSupprimerContrat_clicked()
{
  // TO DO (étape 13)
    int ind;
    if ((ind = getIndiceContratSelectionne()) == -1)
    {
        dialogueErreur("Eureur de sélection", "Aucun contrat n'est séléctionner");
        return;
    }
    
    Iterateur <Contrat> itctr(Garage::getInstance().getIterateurContrat());
    Garage::getInstance().supprimeContratParIndice(ind);
    //on_pushButtonNouveauProjet_clicked();
    videTableContrats();

    for(itctr.reset();!itctr.end();itctr++)
    {
        Contrat ct = (Contrat)itctr;
        ajouteTupleTableContrats(ct.Tuple());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ApplicGarageWindow::on_pushButtonVisualiserVoiture_clicked()
{
  // TO DO (étape 13)
    int ind;
    if ((ind = getIndiceContratSelectionne()) == -1)
    {
        dialogueErreur("Eureur de sélection", "Aucun contrat n'est séléctionner");
        return;
    }
    on_pushButtonNouveauProjet_clicked();

    string nom = Garage::getInstance().getContrat(ind).getNomProjet();
    setNomProjetEnCours(nom);
    nom = nom + ".car";
    Garage::getProjetEnCours().Load(nom);
    Modele m = Garage::getProjetEnCours().getModele();
    string nomModele = m.getNom();
    Moteur moteur = m.getMoteur();
    int mot = 0;
    switch(moteur)
    {
        case Essence: mot = 0;
            break;
        case Diesel: mot = 1;
            break;
        case Electrique: mot = 2;
            break;
        case Hybride: mot = 3;
            break;
    }
    ApplicGarageWindow::setModele(nomModele.c_str(), m.getPuissance(), mot, m.getPrixDeBase(), m.getImage().c_str());

    for(int i=0; i<5; i++)
    {
        if(Garage::getProjetEnCours()[i] != NULL)
        {
            Option op = *(Garage::getProjetEnCours()[i]);
            setTableOption(i, op.getCode(), op.getIntitule(), op.getPrix());
        }
    }

    ApplicGarageWindow::setPrix(Garage::getProjetEnCours().getPrix());
    
}

