// Je tiens a préciser que ce code ne respecte pas (en majorité) la vue schématique (ci j'ai pas oublié de la mettre) du word (exemple les pins sont différent sur le code et sur la vue schématique)
// Dans ce programme la reconnaissance vocale n'est pas encore prise en compte
// Il ce peut que ce programme ne fonctionne pas correctement car je n'ai pas pu le tester dans son entièreté
// Le code individuel pour les modules (4 moteurs + 2 L293D) et (capteur à ultrason + 2 LED) fonctionne
// Je n'ai pas pu tester le code pour le capteur IR (je n'avais pas le matériel)
// Ce code à été pensé pour être téléversé vers une carte arduino MEGA2560 R3 il se peut qu'il y est des modifications à faire sur le code
// Ce programme possède : - les commandes moteurs (vitesse et sens)
//                        - Les commandes Led (allumées ou éteintes)
//                        - Le suivit de ligne avec double capteur IR
//                        - La reconnaissance d'obstacle en fonction de leur distance
// Ce code est un code optionnel est pas le modèle de référence


// Moteur1 = moteur avant droite
// Moteur2 = moteur avant gauche
// Moteur3 = moteur arrière droite
// Moteur4 = moteur arrière gauche

// Pin capteurs infrarouge
int capteurdroite = 6;
int capteurgauche = 7;

// Pin capteur à ultrason
const int trigger = 9;
const int echo = 10;
int distance;
long duree;

// Pin LED
int ledrouge = 11;
int ledverte = 12;

// Pin moteurs
int moteur1_Pont-H = 2; // le pont-H permet d'alimenter ou non le dipôle (ici le moteur).
int moteur1_sortie1 = 53; // les broches 53 et 52 permettent de contrôler le Pont-H et donc le sens de circulation du courant. le moteur peut donc aller dans un sens ou dans l'autre
int moteur1_sortie2 = 52;

int moteur2_Pont-H = 3;
int moteur2_sortie1 = 51;
int moteur2_sortie2 = 50;

// J'ai rajouté 2 moteurs car j'avais fais mon prototype avec 4 moteurs mais le programme devrait marché avec 2 moteurs en modifiant légèrement le code
int moteur3_Pont-H = 4;
int moteur3_sortie1 = 49;
int moteur3_sortie2 = 48;

int moteur4_Pont-H = 5;
int moteur4_sortie1 = 47;
int moteur4_sortie2 = 46;


void setup(){
  //initialisation capteur IR
  pinMode(capteurdroite,INPUT);
  pinMode(capteurgauche,INPUT);

  //initialisation capteur à ultrason
  pinMode(trigger,OUTPUT);
  pinMode(echo, INPUT);

  //initialisation LED
  pinMode(ledrouge,OUTPUT);
  pinMode(ledverte,OUTPUT);

  //initialisation des moteurs
  pinMode(moteur1_Pont-H, OUTPUT);
  pinMode(moteur1_sortie1, OUTPUT);
  pinMode(moteur1_sortie2, OUTPUT);

  pinMode(moteur2_Pont-H, OUTPUT);
  pinMode(moteur2_sortie1, OUTPUT);
  pinMode(moteur2_sortie2, OUTPUT);

  pinMode(moteur3_Pont-H, OUTPUT);
  pinMode(moteur3_sortie1, OUTPUT);
  pinMode(moteur3_sortie2, OUTPUT);

  pinMode(moteur4_Pont-H, OUTPUT);
  pinMode(moteur4_sortie1, OUTPUT);
  pinMode(moteur4_sortie2, OUTPUT);
}

void loop(){

  int valeur_capteurdroite = digitalRead(capteurdroite);
  int valeur_capteurgauche = digitalRead(capteurgauche);

  if ((valeur_capteurdroite) && (!valeur_capteurgauche)){ // Noir à gauche et blanc à droite (bande de scotch noir)
    Gauche(); // tourne à gauche
    DetcObstacle(); // fonction pour détecter les obstacles
  }
  else if ((!valeur_capteurdroite) && (valeur_capteurgauche)){ // Blanc à gauche et noir à droite
    Droite(); // tourne à droite
    DetcObstacle();
  }
  else{ // Si aucun des deux alors ...
    avancer()// les moteurs avancent
    DetcObstacle();
  }
  

  digitalWrite(ledverte,HIGH); // LED vert allumée et rouge éteinte
  digitalWrite(ledrouge,LOW);

  void avancer(){ // fonction qui permet au moteurs d'avancer
    InitMoteur1(255, true);
    InitMoteur2(255, true);
    InitMoteur3(255, true);
    InitMoteur4(255, true);
  }

  void Stopper(){ // fonction qui permet au moteurs de se stopper
    InitMoteur1(0, true);
    InitMoteur2(0, true);
    InitMoteur3(0, true);
    InitMoteur4(0, true);
  } 

/*  void Reculer(){ fonction qui permet au moteurs de reculer (Ici la fonction n'est pas utilisée)
    InitMoteur1(255, false);
    InitMoteur2(255, false);
    InitMoteur3(255, false);
    InitMoteur4(255, false);
  } 
*/

  void Droite(){
    InitMoteur1(255, true);
    InitMoteur2(255, false);
    InitMoteur3(255, true);
    InitMoteur4(255, false);
  }

  void Gauche(){
    InitMoteur1(255, false);
    InitMoteur2(255, true);
    InitMoteur3(255, false);
    InitMoteur4(255, true);
  }

  //Fonctions qui set les paramètres des différents moteurs
  void InitMoteur1(int speed, boolean sens){ // boucle fonction "InitMoteur1" création de var speed (contrôle vitesse) et boolean sens (contrôle sens courant)
    analogWrite(moteur1_Pont-H, speed); // indique la vitesse du moteur
    digitalWrite(moteur1_sortie1, ! sens); // indique le sens dans lequel va tourner le moteur (sens + sens = moteur stoppé) - (! sens + ! sens = moteur stoppé) - (sens + ! sens = moteur tourne dans un sens) - (! sens + sens = moteur tourne dans l'autre sens)
    digitalWrite(moteur1_sortie2, sens);
  }

  void InitMoteur2(int speed, boolean sens){
    analogWrite(moteur2_Pont-H, speed);
    digitalWrite(moteur2_sortie1, ! sens);
    digitalWrite(moteur2_sortie2, sens);
  }

  void InitMoteur3(int speed, boolean sens){
    analogWrite(moteur3_Pont-H, speed);
    digitalWrite(moteur3_sortie1, ! sens);
    digitalWrite(moteur3_sortie2, sens);
  }

  void InitMoteur4(int speed, boolean sens){
    analogWrite(moteur4_Pont-H, speed);
    digitalWrite(moteur4_sortie1, ! sens);
    digitalWrite(moteur4_sortie2, sens);
  }


  // Fonction capteur à ultrasons

  void DetcObstacle(){
    digitalWrite(trigger, LOW); 
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH); // envoie un signal durant 10us
    delayMicroseconds(10);
    digitalWrite(trigger, LOW); // arrête l'envoie du signal
    duree = pulseIn(echo, HIGH); // lecture et calcul du temps de réception du signal (en us)

    distance = duree*0.034/2; // formule pour avoir la distance (d=t*v) : distance(en cm) = durée(en us) * vitesse(du son en cm par us) / 2 (distance total divisé (allé retour signal))
    delay(10);
  }

  if (distance <= 10){ // Si la distance est égal ou inférieur à 10 alors... + tant que distance est égal ou inférieur à 10 alors...
//  int compteur = 0; // fonction qui permet au robot de reculer, si un obstacle lui barre la route pendant 9s (Je sais ça sert à rien parce qu'il est censé suivre une ligne et pas contourner les obstales mais si le programme change ça peut toujours servir. Donc je laisse ce code)
    while (distance <= 10){ // Alors oui j'aurais pu directement faire un while mais je fais comme ça pour set une var avant le while
      Stopper();  // arrête tous les moteurs et attends 3s
      digitalWrite(ledverte,HIGH); // LED vert éteinte et rouge allumée
      digitalWrite(ledrouge,LOW);
      delay(3000);
//    compteur++;
//    if ( compteur = 3 ){ // donc si le robot à attendu pendant 9s
//       Reculer(); // les moteurs reculent pendant 2s l'obstacles n'est plus visible à 10 cm sort du 1er if
//       compteur = 0; // reset le compteur
//       delay(2000);
//    }
    }
  }
}
