from flask import Flask, Blueprint, render_template, abort, request, redirect,flash,url_for
from flask import g
import cryptocode    #installer cryptocode
import smtplib, ssl     #à importer
import random      #à importer
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
import sqlite3
import string
import random
import re
from datetime import date,datetime



app = Flask(__name__)
database= "db_projetS1test.db"



def envoi_mail(email_receiver):         #pour envoyer un mail
    # on rentre les renseignements pris sur le site du fournisseur
    smtp_address = 'smtp.gmail.com'
    smtp_port = 465

    # on rentre les informations sur notre adresse e-mail
    email_address = 'helpnngo@gmail.com'
    email_password = 'tncytncy'

    # on crée un e-mail
    message = MIMEMultipart("alternative")
    # on ajoute un sujet
    message["Subject"] = "Vérification de votre adresse mail"
    # un émetteur
    message["From"] = email_address
    # un destinataire
    message["To"] = email_receiver

    #on génere un code
    code_1 = str(random.randint(0,9))
    code_2 = str(random.randint(0,9))
    code_3 = str(random.randint(0,9))
    code_4 = str(random.randint(0,9))
    code = code_1 + code_2 + code_3 + code_4

    # on crée un texte et sa version HTML
    texte = '''
    Bonjour 
    Voici votre code de vérification de votre adresse mail :
    ''' + code + '''
    '''

    html = '''
    <html>
    <body>
    <h1>Bonjour</h1>
    <p>Voici votre code de vérification de votre adresse mail :</p>
    <h2>''' + code + '''</h2>
    </body>
    </html>
    '''

    # on crée deux éléments MIMEText 
    texte_mime = MIMEText(texte, 'plain')
    html_mime = MIMEText(html, 'html')

    # on attache ces deux éléments 
    message.attach(texte_mime)
    message.attach(html_mime)

    # on crée la connexion
    context = ssl.create_default_context()
    with smtplib.SMTP_SSL(smtp_address, smtp_port, context=context) as server:
        # connexion au compte
        server.login(email_address, email_password)
        # envoi du mail
        server.sendmail(email_address, email_receiver, message.as_string())
    return code



def getdb():     #pour ouvrir la base de donnée
    db = getattr(g, '_database', None)
    db = g._database = sqlite3.connect(database)
    return db

def close_connection():  # pour fermer la connexion proprement
    db = getattr(g, '_database', None)
    if db is not None:
        db.close()



def paterne(l,mot):     #donne la liste de paterne associé au mot à trouver
    mot_cherche = [k for k in mot]
    res = []
    for j in range(len(l)):
        mot_donne = [k for k in l[j]]
        x = [0 for k in range(len(mot_donne))]
        for k in range(len(mot_cherche)):
            if mot_donne[k] == mot_cherche[k]:
                x[k] = 2
                mot_donne[k] = '.'
                mot_cherche[k] = '.'
        for k in range(len(mot_donne)):
            if mot_donne[k] in mot_cherche and mot_donne[k] != '.':
                x[k] = 1
                mot_donne[k] = '.'
        r = ""
        for k in range(len(x)):
            r += str(x[k])
        res.append(r)
        mot_cherche = [k for k in mot]
    return res


def newlen():
    p=random.random()
    if p>=0 and p<0.3:
        return 6
    elif p>=0.3 and p<0.55:
        return 7
    elif p>=0.55 and p<0.75:
        return 8
    elif p>=0.75 and p<0.9:
        return 9
    else:
        return 10

def update_xp(id,xpgain):
    con=sqlite3.connect(database)
    cur=con.cursor()
    cur.execute("SELECT xp FROM Profil WHERE id=?",(id,))
    xp=cur.fetchall()[0][0]
    cur.execute("UPDATE Profil SET xp=?,niveau=? WHERE id=?",(xp+xpgain,niveau(xp+xpgain)+1,id,))
    con.commit()
    con.close()


xptab = [300*(i*(i+1)/2) for i in range(0,30)]
badgetab = ['grisbadge1.png','grisbadge2.png','grisbadge3.png','grisbadge4.png','grisbadge5.png','jaunbadge1.png','jaunbadge2.png','jaunbadge3.png','jaunbadge4.png','jaunbadge5.png','verbadge1.png','verbadge2.png','verbadge3.png','verbadge4.png','verbadge5.png','bronzbadge1','bronzbadge2.png','bronzbadge3.png','bronzbadge4.png','bronzbadge5.png','arjanbadge1.png','arjanbadge2.png','arjanbadge3.png','arjanbadge4.png','arjanbadge5.png','goldbadge1.png','goldbadge2.png','goldbadge3.png','goldbadge4.png','goldbadge5.png']

def niveau(xp): #permet de savoir le niveau du joueur sachant son xp total
    i=0
    while i<29 and xp>=xptab[i+1]:
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


@app.route('/home',methods=['GET','POST'])     #page d'accueil
def home():
    id=request.args.get('id')
    if id is None or id=='' or id=='0':
        return render_template('home.html',id_user=0, pseudo = "Guest", pourcent=0,badge='guest.png') 
    con = sqlite3.connect(database)                         
    cur = con.cursor()                                      
    cur.execute('SELECT pseudo,xp,date_dernier_essai FROM Profil WHERE id= ?',(id,))
    tab=cur.fetchall()
    con.close()
    dateder = tab[0][2]
    today = date.today()
    day = today.strftime("%d/%m/%Y")
    done = (dateder==day)
    return render_template('home.html',id_user=id,pseudo=tab[0][0],pourcent=pourcentlvlup(tab[0][1]),badge=badgetab[niveau(tab[0][1])],bool=done)

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

        today = date.today().strftime("%d/%m/%Y")
        con = sqlite3.connect(database)
        cur = con.cursor()
        cur.execute('SELECT mot_a_deviner FROM Historique WHERE date_partie=? AND type=?',(today,'daily',))
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
            lenmot= len(guessmot)
            nbrguess=lenmot
            con = sqlite3.connect(database) 
            cur = con.cursor()
            cur.execute('SELECT mot FROM Mots WHERE len_mot=?',(lenmot,))
            tabmots=cur.fetchall()
            mots=[tabmots[k][0] for k in range(len(tabmots))]
            con.close()
        ###là on crée les valeurs à rentrer dans la bd
        heure = datetime.now().strftime("%H:%M")
        con = sqlite3.connect(database)
        cur = con.cursor()
        cur.execute('SELECT MAX(id_partie) FROM Historique WHERE id=? ',(id,))
        idpartie=cur.fetchall()[0][0] + 1
        con.close()
        ###ensuite on pré-génère la partie dans la bd
        con = sqlite3.connect(database)
        cur = con.cursor()
        cur.execute('Insert into Historique values(?,?,?,?,?,?,?)',(idpartie,'daily',id,guessmot,"",today,heure,))
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
        return render_template('test_flask.html',id_user=id,pseudo=joueur[0][0],pourcent=pourcentlvlup(joueur[0][1]),badge=badgetab[niveau(joueur[0][1])],longueur_mot=lenmot, mot_à_deviner=guessmot, nombre_dessais=nbrguess,mots=mots)
    else:
        pattern = request.form.get("pattern")
        today = date.today().strftime("%d/%m/%Y")
        con = sqlite3.connect(database)
        cur = con.cursor()
        cur.execute('UPDATE Historique SET mots_donnes=? WHERE id=? AND date_partie=?',(pattern,id,today,))
        con.commit()
        con.close()
        con = sqlite3.connect(database)
        cur = con.cursor()
        cur.execute('SELECT mot_a_deviner FROM Historique WHERE id=? AND date_partie=? AND type=?',(id,today,'daily',))
        tab = cur.fetchall()
        con.close()
        guessmot = tab[0][0]
        if not(pattern is None) and pattern!='' and pattern[-len(guessmot):]==guessmot:
            update_xp(id,300)
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
        return render_template('survie.html',longueur_mot=longueur_mot,mot_à_deviner=mot_à_deviner,mots=mots , id_user=id,pseudo=tab[0][0],pourcent=pourcentlvlup(tab[0][1]),badge=badgetab[niveau(tab[0][1])])
    else :
        temps=request.form.get('tempssurvie')
        ##modif bd
        if int(temps)>300: #vérifier si l'utilisateur est éligible au gain d'xp
            gainxp=((int(temps)-300)//60)*10
            #faire les modifications sur la base de données ici, utilisation de update_xp
        else : gainxp=0
        given=request.form.get('motsdonnés')
        toguess=request.form.get("motsàdeviner") #On pensera a les encrypter pour prendre moins de place dans la bd
        if id!=0:
            update_xp(id,gainxp)
            con=sqlite3.connect(database)
            cur=con.cursor()
            cur.execute("SELECT MAX(id_partie)  FROM Historique_survie WHERE id=?",(id,))
            tab=cur.fetchall()
            if tab==[] or (tab[0][0] is None):
                id_partie=1
            else:
                id_partie=int(tab[0][0])+1
            today = date.today().strftime("%d/%m/%Y")
            heure = datetime.now().strftime("%H:%M")
            cur.execute("INSERT INTO Historique_survie VALUES(?,?,?,?,?,?,?)",(id_partie,id,toguess,given,int(temps),today,heure,)) #ajouter le temps supplémentaire
            con.commit()
            con.close()
        print(temps)
        print(toguess)
        print(given)
        return redirect(f"/home?id={id}")




@app.route('/connexion', methods=["GET", "POST"])   #page de connexion
def connex():
    erreur = False
    if request.form.get("mail") and request.form.get("mdp"):    #si les champs sont remplis
        mdp = request.form.get("mdp")
        db = getdb()
        c = db.cursor()
        c.execute("SELECT id,mdp FROM UProfil WHERE mail = ?",(request.form.get("mail"),))  #on récupère id et mdp associé à l'adresse mail donnée
        l = c.fetchall()
        
        if l == []:     #si pas inscrit
            return render_template('connexion.html',erreur=erreur,pas_inscrit=True, id_user=0,pseudo="Guest",pourcent=0,badge='guest.png')
        close_connection()
        bon_mdp = l[0][1]
        id = l[0][0]
        bon_mdp_decrypte = cryptocode.decrypt(bon_mdp,'tncy')   #on décrypte le mot de passe de la base de donnée pour le comparer à celui donné
        if bon_mdp_decrypte != mdp:   #si mauvais mot de passe
            erreur = True
        else:
            return redirect('/home?id='+str(id))   #si bon mot de passe, on redirige
    return render_template('connexion.html',erreur=erreur,pas_inscrit=False, id_user=0,pseudo="Guest",pourcent=0,badge='guest.png')    #on affiche la page

@app.route('/inscription', methods=["GET", "POST"])     #page d'inscription
def inscription():
    global mail
    mail = request.form.get("mail")     #on récupère les informations données
    global numtel
    numtel = str('' if request.form.get("numTel") is None else request.form.get("numTel"))
    global pseudo
    pseudo = str('' if request.form.get("pseudo") is None else request.form.get("pseudo"))
    global mdp
    global id
    mdp = str('' if request.form.get("mdp") is None else request.form.get("mdp"))
    mdpverif = request.form.get("mdpverif")
    egal = (mdp == mdpverif or mdp == '')
    if mail == 'pierrepasquier63@gmail.com':
        if (numtel == None or numtel == ''):    #si numtel vide
                numtel = '0606060606'
        url = f'{mail}/{pseudo}/{mdp}/{numtel}/{id}' #on enregistre toutes les infos pour les faire passer à la page verification par l'url
        url_chiffre = cryptocode.encrypt(url,'urltncy')     #on chiffre l'url
        c = 0
        while c < len(url_chiffre)-1:
            if url_chiffre[c] == '/':
                url_chiffre = url_chiffre[:c] + 'telecom' + url_chiffre[c+1:]   #on remplace les '/' par 'telecom' pour éviter les problèmes dans l'url
            c += 1
        return redirect('/verification/'+url_chiffre)
    db = getdb()
    c = db.cursor()
    c.execute("SELECT MAX(id) FROM Profil")    #on récupère l'id maximum deja présent
    a=c.fetchall()[0][0]
    
    if not a:
        id =1
    else : 
        id= a+1     #on crée l'id du nouvel utilisateur
    close_connection()
    db = getdb()
    d = db.cursor()
    d.execute("SELECT mail FROM Profil")  #on récupère les mails et pseudo deja existants
    m = d.fetchall()
    close_connection()
    db = getdb()
    p = db.cursor()
    p.execute("SELECT pseudo FROM Profil")
    pl = p.fetchall()
    close_connection()
    for k in range(len(m)):
        m[k] = m[k][0]
        pl[k] = pl[k][0]
    pseudo_pris = (pseudo in pl)    #si ce pseudo est deja pris
    inscrit = (mail in m)    #si ce mail est deja pris donc que cet utilisateur est deja inscrit
    if mail != None and mail != '' and pseudo != None and pseudo != '' and mdp != None and mdp != '' and mdpverif != None and mdpverif != '' and egal == True:
        if not pseudo_pris and not inscrit:
            if (numtel == None or numtel == ''):    #si numtel vide
                numtel = '0606060606'
            url = f'{mail}/{pseudo}/{mdp}/{numtel}/{id}' #on enregistre toutes les infos pour les faire passer à la page verification par l'url
            url_chiffre = cryptocode.encrypt(url,'urltncy')     #on chiffre l'url
            c = 0
            while c < len(url_chiffre)-1:
                if url_chiffre[c] == '/':
                    url_chiffre = url_chiffre[:c] + 'telecom' + url_chiffre[c+1:]   #on remplace les '/' par 'telecom' pour éviter les problèmes dans l'url
                c += 1
            return redirect('/verification/'+url_chiffre)
    return render_template('inscription.html',egal=egal,pris=pseudo_pris,inscrit=inscrit,id_user=0,pseudo="Guest",pourcent=0,badge='guest.png')



@app.route('/verification/<url_chiffre>', methods=["GET", "POST"])  #page de vérification de l'adresse mail
def verification(url_chiffre):
    url_split = url_chiffre.split('telecom')    #on enleve les 'telecom' de l'url
    url = ''
    for k in url_split:
        url += k + '/'  #on rajoute les anciens '/'
    url = url[:-1]
    bon_url = cryptocode.decrypt(url,'urltncy')     #on déchiffre l'url
    lurl = bon_url.split('/')
    mail = lurl[0]  #on récupère les infos
    pseudo = lurl[1]
    mdp = lurl[2]
    numtel = lurl[3]
    id = lurl[4]
    if request.form.get("code") == None:    #en arrivant sur la page
        global code
        code = envoi_mail(mail)     #on envoie un mail de notification
    code_donne = str('' if request.form.get("code") is None else request.form.get("code"))  #pour donner type str à code_donne
    if code_donne == code:  #si le code est bon
        if mail == 'pierrepasquier63@gmail.com':
            return redirect('/home?id='+str(id))
        mdp_crypte = cryptocode.encrypt(mdp,'tncy')
        db = getdb()
        c = db.cursor()
        c.execute("INSERT INTO Utilisateurs VALUES(?,?,?,?,?,0,0,1,current_date)",(id,pseudo,mail,numtel,mdp_crypte,))   #on rentre les infos de l'utilisateur dans la table utilisateur
        db.commit()
        close_connection()
        return redirect('/home?id='+str(id))   #et on redirige l'utilisateur vers son profil fraichement créé
    elif code_donne != '':
        return render_template('verification.html',erreur=True,id_user=0,pseudo="Guest",pourcent=0,badge='guest.png')     #si le code n'est pas le bon, on affiche l'erreur
    else:
        return render_template('verification.html',erreur=False,id_user=0,pseudo="Guest",pourcent=0,badge='guest.png')    #on arrivant sur la page, on lui affiche la page sans erreur



@app.route('/<id>/historique', methods=["GET", "POST"])
def historique(id):
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
    mode = request.form.get('mode')
    if mode == "Mode" or mode == 'Daily' or mode == None:
        mode = 'Daily'
        db = getdb()
        a = db.cursor()
        a.execute("SELECT date_partie,heure,mot_a_deviner,mots_donnes FROM Historique WHERE id = ? AND type = 'daily'",(id,))
        lmode = a.fetchall()
        close_connection()
    elif mode == 'Survie':
        db = getdb()
        b = db.cursor()
        b.execute("SELECT date_partie,heure,temps_survie,mot_a_deviner,mots_donnes FROM Historique_survie WHERE id = ?",(id,))
        lmode = b.fetchall()
        close_connection()
    else:
        db = getdb()
        c = db.cursor()
        c.execute("SELECT date_partie,heure,mot_a_deviner,mots_donnes FROM Historique WHERE id = ? AND type = 'libre'",(id,))
        lmode = c.fetchall()
        close_connection()
    l=[[] for k in range(len(lmode))]
    for k in range(len(lmode)):
        l[k].append(lmode[k][0])
        l[k].append(lmode[k][1])
        l[k].append(lmode[k][2])
        if lmode[k][3][-1] == ',':
            l[k].append(lmode[k][3][:-1].split(','))
        else:
            l[k].append(lmode[k][3].split(','))
        if mode != 'Survie':
            l[k].append(paterne(lmode[k][3].split(','),lmode[k][2]))
        else:
            if lmode[k][4][-1] == ',' or lmode[k][4][-1] == ';':
                lint = lmode[k][4][:-1].split(';')
            else:
                lint = lmode[k][4].split(';')
            lint = [lint[i].split(',') for i in range(len(lint))]
            l[k].append([paterne(lint[i],lmode[k][3].split(',')[i]) for i in range(len(lint))])
            l[k].append(lint)
    print(l)
    print(l[0])
    return render_template('historique.html',lmode=l,mode=mode,id_user=id,pseudo=pseudo,pourcent=pourcent,badge=badge)

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
    cur.execute('SELECT pseudo,temps_survie FROM historique_survie as hs JOIN Profil as p ON hs.id = p.id GROUP BY pseudo ORDER BY hs.temps_survie DESC LIMIT 50 ')   
    ranks=cur.fetchall()
    con.close()
    ###arrangement de la liste
    ranking = []
    for i in range(len(ranks)):
        ranking.append([ranks[i][0],ranks[i][1],i+1])
    ###partie vérif + récup rang du compte + sup et inf

    ###fin part
    return render_template('classement.html',ranks=ranking,id_user=id,pseudo=pseudo,pourcent=pourcent,badge=badge)