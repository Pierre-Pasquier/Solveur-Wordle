from flask import Flask, render_template, request, redirect
import random
import sqlite3
from datetime import date,datetime

app = Flask(__name__)
database= "db_projetS1.db"

xptab = [300*i for i in range(0,30)]
badgetab = ['grisbadge1.png','grisbadge2.png','grisbadge3.png','grisbadge4.png','grisbadge5.png','jaunbadge1.png','jaunbadge2.png','jaunbadge3.png','jaunbadge4.png','jaunbadge5.png','verbadge1.png','verbadge2.png','verbadge3.png','verbadge4.png','verbadge5.png','bronzbadge1','bronzbadge2.png','bronzbadge3.png','bronzbadge4.png','bronzbadge5.png','arjanbadge1.png','arjanbadge2.png','arjanbadge3.png','arjanbadge4.png','arjanbadge5.png','goldbadge1.png','goldbadge2.png','goldbadge3.png','goldbadge4.png','goldbadge5.png']

def niveau(xp): #permet de savoir le niveau du joueur sachant son xp total
    i=0
    while i<30 and xp>xptab[i]:
        i+=1
    return(i) #correspond à la position du badge associé dans badgetab

def pourcentlvlup(xp): #renvoie le % d'xp avant prochain lvl -> renvoie 100% si niveau max
    i = niveau(xp)
    if i==29:
        return(100)
    else:
        return(int(100*(xp-xptab[i])/xptab[i+1]))
        


@app.route('/')      #redirection vers page d'accueil
def redirection():
    return redirect('/home')


@app.route('/à_propos')      #page à propos
def apropos():
    id=request.args.get('id')
    if id is None or id=='' or id=='0':
        return render_template('apropos.html',id_user=0, pseudo = "Guest", pourcent=0,badge='guest.png') 
    con = sqlite3.connect(database)                         
    cur = con.cursor()                                      
    cur.execute('SELECT pseudo,xp FROM Profil WHERE id= ?',(id,))
    tab=cur.fetchall()
    con.close()
    return render_template('apropos.html',id_user=id,pseudo=tab[0][0],pourcent=pourcentlvlup(tab[0][1]),badge=badgetab[niveau(tab[0][1])])


@app.route('/contactez_nous')   #page contactez nous
def contactez():
    id=request.args.get('id')
    if id is None or id=='' or id=='0':
        return render_template('contacteznous.html',id_user=0, pseudo = "Guest", pourcent=0,badge='guest.png') 
    con = sqlite3.connect(database)                         
    cur = con.cursor()                                      
    cur.execute('SELECT pseudo,xp FROM Profil WHERE id= ?',(id,))
    tab=cur.fetchall()
    con.close()
    return render_template('contacteznous.html',id_user=id,pseudo=tab[0][0],pourcent=pourcentlvlup(tab[0][1]),badge=badgetab[niveau(tab[0][1])])


@app.route('/home')     #page d'accueil
def home():
    id=request.args.get('id')
    if id is None or id=='' or id=='0':
        return render_template('home.html',id_user=0, pseudo = "Guest", pourcent=0,badge='guest.png') #voir un paramètre de disable ? permettant de disable telle ou telle option comme
    con = sqlite3.connect(database)                         #les historiques puisque liés au compte? ATTENTION : faudra faire gaffe niveau bd à ne pas save les parties guests
    cur = con.cursor()                                      #faut pas que j'oublie de mettre guest.png sur git (manuellement bcz pas sur la vm)
    cur.execute('SELECT pseudo,xp,date_dernier_essai FROM Profil WHERE id= ?',(id,))
    tab=cur.fetchall()
    con.close()
    dateder = tab[0][2]
    today = date.today()
    day = today.strftime("%Y-%m-%d")
    done = (dateder==day)
    return render_template('home.html',id_user=id,pseudo=tab[0][0],pourcent=pourcentlvlup(tab[0][1]),badge=badgetab[niveau(tab[0][1])],daily=done)

@app.route('/<id>/daily',methods=['GET','POST'])
def daily(id):
    ###d'abord verif si mot du jour existe (joueur ayant déjà joué today) et récup
    ###sinon générer mot random (et sa longueur)
    ###cas guest à voir, car idk comment enregistrer fait ou pas fait du daily (empecher sans connexion? sad)
    if id is None or id=='' or id=='0':
        return render_template('home.html',id_user=0, pseudo = "Guest", pourcent=0,badge='guest.png') 
    
    con = sqlite3.connect(database)                         
    cur = con.cursor()                                      
    cur.execute('SELECT pseudo,xp,date_dernier_essai FROM Profil WHERE id= ?',(id,))
    joueur=cur.fetchall()
    con.close()

    today = date.today().strftime("%Y-%m-%d")
    con = sqlite3.connect(database)
    cur = con.cursor()
    cur.execute('SELECT mot_a_deviner FROM Historique WHERE date=? AND type=?',(today,'daily',))
    tab = cur.fetchall()
    con.close()
    a_generer = (tab==[])
    if a_generer:
        ###on génère un mot 
        guessmot = "MEILLEUR"
        lenmot=8
    else:
        mot = tab[0][0]
    ###là on crée les valeurs à rentrer dans la bd
    heure = datetime.now().strftime("%H:%M:%S")
    con = sqlite3.connect(database)
    cur = con.cursor()
    cur.execute('Select MAX(SELECT id FROM Historique WHERE id_joueur=? AND type=?) FROM Sondages',(id,'daily',))
    idpartie=cur.fetchall()
    con.close()
    ###ensuite on pré-génère la partie dans la bd
    con = sqlite3.connect(database)
    cur = con.cursor()
    cur.execute('Insert into Historique values(?,"daily",?,?,?,?,?)',(idpartie,id,[],guessmot,today,heure))
    con.commit()
    con.close()
    ###maintenant on va changer la date du dernier essai de daily du joueur
    con = sqlite3.connect(database)
    cur = con.cursor()
    cur.execute('UPDATE Profil SET date_dernier_essai=? WHERE id=?',(today,id,))
    con.commit()
    con.close()
    ###puis on render la page de partie, dans laquelle il faudra en sortie mettre à jour la bd pour compléter la partie en historique
    ###voir pour résultat dans la page historique : exemple partie quittée en cours -> partie pas complétée donc vide?
    return render_template('daily.html',id_user=id,pseudo=joueur[0][0],pourcent=pourcentlvlup(joueur[0][1]),badge=badgetab[niveau(joueur[0][1])],longueur_mot=lenmot, mot_à_deviner=guessmot, nombre_dessai=6)

@app.route('/<id>/pl')
def partie_libre(id):
    longueur_mot=8 #modif prochainement
    nombre_dessais=10
    mot_à_deviner="FUSIONNE"
    return render_template('test_flask.html',longueur_mot=longueur_mot,mot_à_deviner=mot_à_deviner,nombre_dessais=nombre_dessais)


@app.route('/<id>/survie')
def mode_survie(id):

    con = sqlite3.connect(database) 
    cur = con.cursor()
    cur.execute('SELECT mot FROM Mots')
    tabmots=cur.fetchall()
    mots=[tabmots[k][0] for k in range(len(tabmots))]
    con.close()

    longueur_mot=random.randint(6,10)
    con = sqlite3.connect(database) 
    cur = con.cursor()
    cur.execute('SELECT mot FROM Mots WHERE len_mot=?',(longueur_mot,))
    tabchoice=cur.fetchall()
    motlen=[tabchoice[k][0] for k in range(len(tabchoice))]
    borne=random.randint(0,len(motlen)-1)
    mot_à_deviner=motlen[borne]
    return render_template('survie.html',longueur_mot=longueur_mot,mot_à_deviner=mot_à_deviner,mots=mots)
