from flask import Flask, render_template, request, redirect


app = Flask(__name__)
#database= "db_projetS2.db"

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