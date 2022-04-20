from flask import Flask, render_template, request, redirect, url_for
import random
import sqlite3
from datetime import date,datetime

app = Flask(__name__)
database= "db_projetS1test.db"

def newlen(): ### fonction qui renvoie une longueur avec 
    p=random.random()
    if p>=0 and p<0.3: # 30% : 6
        return 6
    elif p>=0.3 and p<0.55: #25% : 7
        return 7
    elif p>=0.55 and p<0.75: #20% : 8
        return 8
    elif p>=0.75 and p<0.9: #15% : 9
        return 9
    else:  #10% : 10
        return 10

def update_xp(id,xpgain):
    con=sqlite3.connect(database)
    cur=con.cursor()
    cur.execute("SELECT xp FROM Profil WHERE id=?",(id,))
    xp=cur.fetchall()[0][0]
    cur.execute("UPDATE Profil SET xp=? AND niveau=? WHERE id=?",(xp+xpgain,niveau(xp+xpgain),id,))
    con.commit()
    con.close()


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
    cur = con.cursor()                                      
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
    if request.method=='GET':
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
            lenmot = random.randrange(6,11) ###à la limite, ici on peut décider de l'aléatoire de la longueur du mot
            nbrguess= lenmot
            con = sqlite3.connect(database) 
            cur = con.cursor()
            cur.execute('SELECT mot FROM Mots WHERE len_mot=?',(lenmot,))
            tabmots=cur.fetchall()
            mots=[tabmots[k][0] for k in range(len(tabmots))]
            con.close()
            i = random.randrange(0,len(mots))
            guessmot = mots[i]
        else:
            guessmot = tab[0][0]
        ###là on crée les valeurs à rentrer dans la bd
        heure = datetime.now().strftime("%H:%M:%S")
        con = sqlite3.connect(database)
        cur = con.cursor()
        cur.execute('Select MAX(SELECT id FROM Historique WHERE id_joueur=? AND type=?) FROM Sondages',(id,'daily',))
        idpartie=cur.fetchall()[0][0] + 1
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
        ###voir pour résultat dans la page historique : exemple partie quittée en cours -> partie pas complétée donc vide? -> 
        ###-> possibilité de ne pas prendre en compte les parties vides à l'affichage de l'historique
        return render_template('test_flask.html',id_user=id,pseudo=joueur[0][0],pourcent=pourcentlvlup(joueur[0][1]),badge=badgetab[niveau(joueur[0][1])],longueur_mot=lenmot, mot_à_deviner=guessmot, nombre_dessai=nbrguess)
    else:
        ###voir ici pour la méthode post, aka potentiellement le retour de la partie?
        return redirect(f"/home?id={id}")

@app.route('/<id>/pl',methods=['GET','POST'])
def partie_libre(id):
    ###partie de récupération des paramètres pour template
    if id is None or id=='' or id=='0':
        id=0
        pseudo="Guest"
        pourcent = 0
        badge = 'guest.png'
    else:
        con = sqlite3.connect(database)                         
        cur = con.cursor()                                      
        cur.execute('SELECT pseudo,xp FROM Profil WHERE id= ?',(id,))
        tab=cur.fetchall()
        con.close()
        pseudo = tab[0][0]
        pourcent = pourcentlvlup(tab[0][1])
        badge = badgetab[niveau(tab[0][1])]
    ###fin récup
    if request.method=='POST':
        ###besoin de post pour lancer partie (car params de home) et pour récup partie (résultats)
        ###partie de vérif si on vient d'une partie terminée
        pattern=request.form.get("pattern")
        print(pattern)
        if not pattern is None:
            return redirect(f"/home?id={id}")
            #ici faut modif la bd
        ###fin de vérif
        ###partie de récupération des paramètres du home
        if request.form.get("lenmot") and request.form.get("nbrguess"):
            lenmot = int(request.form.get("lenmot"))
            nbrguess = int(request.form.get("nbrguess"))
            ###sous partie pour aléatoire et selon mot :
            if lenmot==-1:
                lenmot = random.randrange(6,11)
            if nbrguess==-1:
                nbrguess = random.randrange(6,11)
            if nbrguess==-2:
                nbrguess = lenmot
        else:
            lenmot = random.randrange(6,11) ##à la limite, ici on peut décider de l'aléatoire de la longueur du mot
            nbrguess= lenmot
        ###fin de la récupération, ajout ci dessous de l'utilisation des params
        con = sqlite3.connect(database) 
        cur = con.cursor()
        cur.execute('SELECT mot FROM Mots WHERE len_mot=?',(lenmot,))
        tabmots=cur.fetchall()
        mots=[tabmots[k][0] for k in range(len(tabmots))]
        con.close()
        ###sélection aléatoire dans la table des mots de longeurs lenmot :
        i = random.randrange(0,len(mots))
        mot_à_deviner = mots[i]
        ###
        return render_template('test_flask.html',longueur_mot=lenmot,mot_à_deviner=mot_à_deviner,nombre_dessais=nbrguess,mots=mots,id_user=id,pseudo=pseudo,pourcent=pourcent,badge=badge)
    else :
        pattern=request.form.get('pattern')
        #modif bd et vérifier si id!=0 pour save
        print(pattern) ##pour tester
        return redirect(f"/home?id={id}")


@app.route('/<id>/survie',methods=['GET','POST'])
def mode_survie(id):
    if request.method=='GET':
        con = sqlite3.connect(database) 
        cur = con.cursor()
        cur.execute('SELECT mot FROM Mots')
        tabmots=cur.fetchall()
        mots=[tabmots[k][0] for k in range(len(tabmots))]
        con.close()

        longueur_mot=newlen()
        con = sqlite3.connect(database) 
        cur = con.cursor()
        cur.execute('SELECT mot FROM Mots WHERE len_mot=?',(longueur_mot,))
        tabchoice=cur.fetchall()
        motlen=[tabchoice[k][0] for k in range(len(tabchoice))]
        borne=random.randint(0,len(motlen)-1) ##à la limite, ici on peut décider de l'aléatoire de la longueur du mot?
        mot_à_deviner=motlen[borne]
        return render_template('survie.html',longueur_mot=longueur_mot,mot_à_deviner=mot_à_deviner,mots=mots , id_user=id)
    else :
        temps=request.form.get('tempssurvie')
        ##modif bd
        gainxp=((int(temps)-300)//60)*10
        print(temps)
        print(gainxp)
        return redirect(f"/home?id={id}")


@app.route('/<id>/classement')
def classement(id):
    if id is None or id=='' or id=='0':
        id = 0
        pseudo = "Guest"
        pourcent = 0
        badge = 'guest.png'
    else:
        con = sqlite3.connect(database)                         
        cur = con.cursor()                                      
        cur.execute('SELECT pseudo,xp FROM Profil WHERE id= ?',(id,))
        tab=cur.fetchall()
        con.close()
        pseudo = tab[0][0]
        pourcent = pourcentlvlup(tab[0][1])
        badge = badgetab[niveau(tab[0][1])]
    ###partie récup des 50 premiers du classement
    con = sqlite3.connect(database)
    cur = con.cursor()
    cur.execute('SELECT pseudo, temps_survie FROM histo_survie as hs JOIN Profil as p ON hs.id_joueur = p.id ORDER BY hs.temps_survie DESC LIMIT 50')   
    ranks=cur.fetchall()
    con.close()
    ranks.append([i for i in range(1,51)])
    ###partie vérif + récup rang du compte + sup et inf

    ###fin part
    return render_template('classement.html',ranks,   id_user=id, pseudo=pseudo,pourcent=pourcent,badge=badge)