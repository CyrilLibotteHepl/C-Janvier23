#ifndef APPLICGARAGEWINDOW_H
#define APPLICGARAGEWINDOW_H

#include <QMainWindow>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <list>
#include <new>

#include "Modele.h"
#include "Voiture.h"
#include "Option.h"
#include "Personne.h"
#include "Intervenant.h"
#include "Client.h"
#include "Employe.h"
#include "Exception.h"
#include "OptionException.h"
#include "PasswordException.h"
#include "Vecteur.h"
#include "Iterateur.h"
#include "VecteurTrie.h"
#include "Garage.h"
#include "Contrat.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class ApplicGarageWindow; }
QT_END_NAMESPACE

class ApplicGarageWindow : public QMainWindow
{
    Q_OBJECT

public:
    ApplicGarageWindow(QWidget *parent = nullptr);
    ~ApplicGarageWindow();

    // Titre de la fenêtre
    void setTitre(string titre);

    // Gestion des roles
    void setRole(int val = 3);

    // Table des options du projet en cours
    void setTableOption(int indice, string code = "", string intitule = "", float prix = -1.0);
    void videTableOption();
    int  getIndiceOptionSelectionneeTable() const;

    // Combobox des modèles disponibles
    void ajouteModeleDisponible(const char *nom, float prixDeBase);
    void videComboboxModelesDisponibles();
    int  getIndiceModeleSelectionneCombobox() const;

    // Combox des options disponibles
    void ajouteOptionDisponible(string intitule, float prix);
    void videComboboxOptionsDisponibles();
    int  getIndiceOptionSelectionneeCombobox() const;

    // Gestion du Projet en cours
    void   setNomProjetEnCours(string nom);
    string getNomProjetEnCours() const;
    void   setModele(string nom, int puissance, int moteur, float prixDeBase,string image);
    void   setPrix(float prix);

    // Table des employes
    void ajouteTupleTableEmployes(string tuple);
    void videTableEmployes();
    int getIndiceEmployeSelectionne();

    // Table des clients
    void ajouteTupleTableClients(string tuple);
    void videTableClients();
    int getIndiceClientSelectionne();

    // Table des contrats
    void ajouteTupleTableContrats(string tuple);
    void videTableContrats();
    int getIndiceContratSelectionne();

    // Boites de dialogues
    void dialogueMessage(const char *titre, const char *message);
    void dialogueErreur(const char *titre, const char *message);
    string dialogueDemandeTexte(const char *titre, const char *question);
    int dialogueDemandeInt(const char *titre, const char *question);
    float dialogueDemandeFloat(const char *titre, const char *question);

private slots:
    void closeEvent(QCloseEvent *event);

    void on_actionNouveau_modele_triggered();
    void on_actionNouvelle_option_triggered();
    void on_actionAjouterEmploye_triggered();
    void on_actionSupprimerEmploye_par_numero_triggered();
    void on_actionSupprimerEmploye_selection_triggered();
    void on_actionAjouterClient_triggered();
    void on_actionSupprimerClient_par_numero_triggered();
    void on_actionSupprimerClient_selection_triggered();
    void on_actionLogin_triggered();
    void on_actionQuitter_triggered();
    void on_actionLogout_triggered();
    void on_actionReset_Mot_de_passe_triggered();

    void on_pushButtonChoisirModele_clicked();
    void on_pushButtonAjouterOption_clicked();
    void on_pushButtonSupprimerOption_clicked();
    void on_pushButtonReduction_clicked();
    void on_pushButtonNouveauProjet_clicked();
    void on_pushButtonOuvrirProjet_clicked();
    void on_pushButtonEnregistrerProjet_clicked();
    void on_pushButtonNouveauContrat_clicked();
    void on_pushButtonSupprimerContrat_clicked();
    void on_pushButtonVisualiserVoiture_clicked();

private:
    Ui::ApplicGarageWindow *ui;
};
#endif // APPLICGARAGEWINDOW_H
