from flask import Flask, Blueprint, render_template, abort, request, redirect,flash
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
import datetime


app = Flask(__name__)
database= "db_projetS2.db"



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
    return render_template('home_connect.html',id_user=id)


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
            return render_template('connexion.html',erreur=erreur,pas_inscrit=True)
        close_connection()
        bon_mdp = l[0][1]
        id = l[0][0]
        bon_mdp_decrypte = cryptocode.decrypt(bon_mdp,'tncy')   #on décrypte le mot de passe de la base de donnée pour le comparer à celui donné
        if bon_mdp_decrypte != mdp:   #si mauvais mot de passe
            erreur = True
        else:
            return redirect('/'+str(id)+'/home')   #si bon mot de passe, on redirige
    return render_template('connexion.html',erreur=erreur,pas_inscrit=False)    #on affiche la page

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
    return render_template('inscription.html',egal=egal,pris=pseudo_pris,inscrit=inscrit)



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
            return redirect('/'+str(id)+'/home')
        mdp_crypte = cryptocode.encrypt(mdp,'tncy')
        db = getdb()
        c = db.cursor()
        c.execute("INSERT INTO Utilisateurs VALUES(?,?,?,?,?,0,0,1,current_date)",(id,pseudo,mail,numtel,mdp_crypte,))   #on rentre les infos de l'utilisateur dans la table utilisateur
        db.commit()
        close_connection()
        return redirect('/'+str(id)+'/home')   #et on redirige l'utilisateur vers son profil fraichement créé
    elif code_donne != '':
        return render_template('verification.html',erreur=True)     #si le code n'est pas le bon, on affiche l'erreur
    else:
        return render_template('verification.html',erreur=False)    #on arrivant sur la page, on lui affiche la page sans erreur
