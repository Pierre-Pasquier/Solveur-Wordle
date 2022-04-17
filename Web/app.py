from flask import Flask, render_template, request, redirect
import random
import sqlite3

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
    id = request.args.get('id')
    return render_template('apropos.html',id_user=id)


@app.route('/contactez_nous')   #page contactez nous
def contactez():
    id = request.args.get('id')
    return render_template('contacteznous.html',id_user=id)


@app.route('/home')     #page d'accueil
def home():
    id=request.args.get('id')
    if id is None or id=='':
        return render_template('home.html') 
    con = sqlite3.connect(database) 
    cur = con.cursor()
    cur.execute('SELECT pseudo,xp FROM Profil WHERE id= ?',(id,))
    tab=cur.fetchall()
    con.close()
    return render_template('home.html',id_user=id,pseudo=tab[0][0],pourcent=pourcentlvlup(tab[0][1]),badge=badgetab[niveau(tab[0][1])])

@app.route('/<id>/pl')
def partie_libre(id):
    longueur_mot=8 #modif prochainement
    nombre_dessais=8
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
