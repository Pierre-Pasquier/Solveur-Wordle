# _**PPII «Projet Pluridisciplinaire d'Informatique Intégrative» 2 (2021-2022)**_

## Membres du groupe :
- DE TOFFOLI Julien
- KIEFFER Thomas
- IORI-GINGEMBRE Nathan
- PASQUIER Pierre

# **PARTIE WEB** 

## Description du projet :

TuNom dispose de 3 modes de jeux qui ont chacun pour principal objectif de trouver des mots à la manière d’un motus :

- Tout d'abord un mode de **jeu libre**  classique qui vous permettra de chercher les mots d’une taille et d'un  nombre d'essais paramétrables. 

- Si vous êtes plutôt d’esprit compétiteur vous pourrez choisir le **mode survie** qui donne la possibilité aux joueurs de se départager afin de savoir qui est vraiment le meilleur. Son principe est simple ,les joueurs disposent d’un temps initial de 5 minutes et qui augmentera selon votre efficacité à trouver chaque mot. La partie s’arrête lorsque vous arrivez à 0, l’objectif est alors simple : tenir le plus longtemps possible !

- Pour ceux jouant de temps en temps, le **mode daily** c'est-à-dire un mode journalier permettant avec un seul essai par jour d'essayer de deviner le mot du jour. 

Ces deux derniers modes de jeu vous permettront de gagner des points d'expérience, qui seront affichés via un badge selon votre niveau et une barre d'expérience selon votre proximité du niveau suivant. Le mode daily vous offrera de l'expérience chaque jour si vous trouvez le mot, tandis que le mode survie vous donnera de l'expérience selon le temps de votre survie.

Vous disposez ainsi de toutes les clefs pour pouvoir profiter au mieux de l’expérience TuNom ! A vous maintenant de mettre vos connaissances et votre mémoire en action au service du meilleur motus vu jusqu’à présent !
	
## Installation :

Afin de pouvoir jouer à TuNom, le joueur doit avant tout se munir des librairies nécessaires ainsi que de lancer le jeu via Flask.

Tout d'abord, une fois le projet entièrement téléchargé dans un dossier, ouvrez un terminal et accédez au dossier contenant le projet. Le code suivant vous permettra d'installer les ressources nécessaires : click==8.1.2, cryptocode==0.1, Flask==2.1.1, importlib-metadata==4.11.3, itsdangerous==2.1.2, Jinja2==3.1.1, MarkupSafe==2.1.1, pycryptodomex==3.14.1, Werkzeug==2.1.1 , zipp==3.7.0

```sh
pip install -r requirements.txt
# installera click==8.1.2, cryptocode==0.1, Flask==2.1.1, importlib-metadata==4.11.3, itsdangerous==2.1.2,
# Jinja2==3.1.1,MarkupSafe==2.1.1, pycryptodomex==3.14.1, Werkzeug==2.1.1 , zipp==3.7.0
```

Ensuite, afin de lancer le serveur, executez le code suivant afin de lancer flask avec l'application web :

```sh
cd Web/
export FLASK_APP=app.py
flask run
```

Une fois ceci fait, il ne vous reste plus qu'à taper dans l'url sur votre navigateur ou de cliquer directement sur cette addresse [http://127.0.0.1:5000/]() afin d'accéder a la page web.


## Comment jouer ?

Maintenant que vous êtes sur le hub du site, vous jouez en tant qu'invité. Cela vous permet d'accéder à partie libre et survie.
Cependant, en vous inscrivant vous pourrez accéder à vos historiques de parties ainsi qu'au mode journalier voir être au top du classement de survie !

Dans le hub pour jouer à un mode, il suffit de cliquer sur lancer la partie dans la case du mode en question. Un cadre avec des cases apparaitra, et vous pourrez en tapant sur votre clavier, y entrer des lettres.
Entrez donc un mot, puis en tapant la touche "entrée", vous pourrez voir si vous avez trouvé le bon mot !
Les lettres se colorant en vert sont bien placées, celles en jaune sont dans le mot mais au mauvais endroit, et celle restant en gris indiquent que la lettre ne fait pas partie du mot. Vous pouvez donc très bien avoir un E vert, un E jaune et un E gris, signifiant qu'un E est bien placé, qu'un deuxième E est dans le mot, mais qu'il n'y a que deux E.

Une fois la partie terminée vous entendrez une musique vous indiquant que vous avez gagné la partie !


# **PARTIE SOLVEUR** 

## Description :

Notre solveur de motus en C est capable via un dictionnaire donné dans des fichiers .txt de trouver les mots à partir des motifs que reçoit le joueur. Les longueurs possibles dépendent des dictionnaires de l'utilisateur, et le choix de la longueur pour le solveur se fait via le fichier wsolf.txt où doit être écrit la taille du mot, ou alors en paramètre du solveur lors de son exécution.
La méthode utilisée est celle du pré traitement, ainsi notre application est avantageuse pour une grande quantité de partie sur un dictionnaire fixé. 

## Installation :

Comme pour la partie web, il faut tout d'abord cloner le projet, puis une fois déplacé dans le dossier Solveur, il suffira d'exécuter le makefile et de lancer l'application dans le terminal. Nous avons un dictionnaire préparé à l'avance pour l'occasion, mais si vous souhaitez le changer, il suffira ensuite (voir utilisation) de relancer l'étape de pré traitement :

```sh
cd projet2-E2/Solveur
make preTT
./preTT
```

Une fois pré traité (ou non si cela a déjà été fait ou est déjà fait), il suffit de lancer dans le terminal le solveur avec ou non une longueur choisie. Si la longueur n'est pas en paramètre, elle sera récupérée dans le fichier wsolf.txt.

```sh
cd projet2-E2/solveur
make solveur
./ solveur (+ longueur optionnelle)
```

## Utilisation :

Afin d'utiliser le solveur, il faut voir comment utiliser ses deux parties. 
La première partie, le pré traitement est optionnelle selon ce que vous souhaitez faire.
Si vous voulez changer de dictionnaire, il faut dans Mots/motN.txt (N étant la longueur des mots dedans) mettre les mots de longueur N de votre dictionnaire dans le fichier, ligne par ligne dans l'ordre lexicographique sans accent avec le nombre de mots en première ligne (voir le format dans les fichiers actuels). Une fois fait il suffit d'exécuter ./preTT et d'attendre quelques minutes que les fichiers de fin de pré traitement soient mis à jour.

Ensuite, pour utiliser la partie solveur, il faut donner la longueur de mot à chercher, soit dans le fichier wsolf.txt, soit en paramètre ./solveur N. Sachez que la longueur en paramètre prend la priorité sur le fichier wsolf et change la longueur dans le fichier.
Enfin, une fois le solveur exécuté, il faudra suivre ses indications afin qu'il sache où vous en êtes. Il vous proposera le premier mot à rentrer, et vous demandera le motif résultant, et ce jusqu'à avoir trouvé le mot.
Il est possible d'ajouter en paramètre après votre motif dans la même ligne -i afin de savoir le nombre de mots restants possibles grâce au motif rentré.
Cependant, sachez que si vos motifs ne sont pas cohérents ou que vous n'utilisez pas correctement le solveur, il vous demandera soit de répéter, soit vous dira qu'une erreur et survenue et s'éteindra.
Finalement, il est possible dans la demande de motif de repondre -1 afin de quitter l'application.
