function testcase(step){ //Pour voir si le mot à l'étape step est vide
    var ell=$('.case');
    for(var k=step*longueur_mot;k<(step+1)*longueur_mot;k++){
        if (ell[k].value==''){
            return false;
        }

    }
    return true;
}
function check_win(mot){
    return mot==mot_à_deviner;
}

function disable(ell){
   for (var k=0;k<ell.length;k++){
       $(ell[k]).prop("disabled",true);
   }
}
function enable(ell){
   for (var k=0;k<ell.length;k++){
       $(ell[k]).prop("disabled",false);
   }

}

function find_cases_débloquées(){
   var L=document.querySelectorAll('.case');
   var ell=[];
   for (var k=0;k<L.length;k++){
       if (!(L[k].disabled)){
           ell.push(L[k]);
       }
   }
   return ell;
}

function get_cases(step){ /**pré-condition étape<nombre de trys max */ //Pour avoir les cases à l'étape step
   var L =document.querySelectorAll(".case");
   var ell=[];
   for (var k=step*longueur_mot;k<(step+1)*longueur_mot;k++){
       ell.push(L[k]);

   }
   return ell;

}
function bonnes_cases(mot,mot2,sequ){ // Détermine la séquence assortie au guess mot par rapport au mot à deviner mot2, séquence des cases vertes
   var cop='';
   var cop2='';
   var cop3='';
   for (var k=0;k<mot.length;k++){
       if (mot[k]==mot2[k]){
           cop3+='2';
           cop+='.';
           cop2+='.';
       }
       else{
           cop+=mot[k];
           cop2+=mot2[k];
           cop3+=sequ[k];
       }
   }
   return [cop,cop2,cop3];


}

function bonnes_lettres(mot,mot2,sequence){ // Détermine la séquence assortie au guess mot par rapport au mot à deviner mot2, séquence des cases jaunes
   var cop='';
   var cop2='';
   var cop3='';
   var tabl=mot2.split("");
   for (var k=0;k<mot.length;k++){
       if (mot[k]!='.' && tabl.includes(mot[k])){
           cop3+='1';
           var index=tabl.indexOf(mot[k]);
           tabl[index]='.';
       }
       else{

           cop3+=sequence[k];

       }
   }
   return cop3;


}

function new_sequence(mot){ //Génère une nouvelle séquence en fonction de la longueur du mot
   var seq='';
   for (var k=0;k<mot.length;k++){
       seq+='0'
   }
   return seq;
}
var toguess=document.querySelector('.toguess');
if (toguess!=null){toguess.value+=mot_à_deviner;}
var étape=0;
var k=0;
var inc=-1;

function sequence(mot,mot2){ /** Attention, si dans le mot à deviner il n'y a qu'un seul L et que dans le mot proposé il y en a plusieurs, n'en mettre que un en jaune */
   var seq='';
   var copy=mot2.slice();
   for (var k=0;k<mot.length;k++){

       if (mot[k]==copy[k]){
           seq+='2';
           copy=copy.replace(mot[k],'.');
 
           
       }
       else if(copy.includes(mot[k])){
           seq+='1';
           copy=copy.replace(mot[k],'.');

       }
       else{
           seq+='0';
       }
   }
   return seq;
   }

function set_cases(ell,motList){ //Affecter aux cases dans ell les valeurs des lettres dans motList
    for (var k=0;k<motList.length;k++){
    if (motList[k]!='.'){
      $(ell[k]).val(motList[k]);  
        }
    }
}
function const_mot(ell){ //Obtenir le mot dans la liste de cases
   var cons='';
   for (var k=0;k<ell.length;k++){
       cons+=ell[k].value;
   }
   return cons;

   
}
var saisie_auto=[];
    for (var k=0;k<longueur_mot;k++){
        saisie_auto.push('.');
    }




function change_colors(ell,sequence){ //Change les couleurs des cases de ell en fonction de la séquence 
    var t= 200;
    
    $(ell).each(function(i)
    {
    var $this = $(this);
          
    setTimeout(function() {
        if (sequence[i]=='1'){
            var audio_lettre_mal_placee = new Audio('/static/Sons motus/motus-lettre-mal-placée.mp3');
            $this.switchClass("","y",300);
            audio_lettre_mal_placee.play();
         }
        else if(sequence[i]=='2'){
        var audio_bonne_lettre = new Audio('/static/Sons motus/motus-lettre-bonne.mp3');
         $this.switchClass("","g",300);
         saisie_auto[i]=$this.val();
         audio_bonne_lettre.play();
         }
         else{
            var audio_mauvaise_lettre = new Audio('/static/Sons motus/motus-mauvaise-lettre.mp3');
             audio_mauvaise_lettre.play();
         }
 
 
    }, t);
          
    t += 195;
 
    })
 
 
    
}




function sleep (time) {
    return new Promise((resolve) => setTimeout(resolve, time));
}










   

$(document).keyup(function (e) //Pour détecter les touches du clavier
{   var L=document.querySelectorAll('.case'); 
       if (e.keyCode >=65 && e.keyCode<=90){ //input lettres
        if (inc!='f'){  
        if (étape+1<nombre_dessais){
            if(inc<(longueur_mot)*(étape+1)-1){
                inc+=1;
            }
        }
        else{
            if (inc<(longueur_mot*nombre_dessais)-1){
                inc+=1;
            }

        }

            
            $(L[inc]).val(String.fromCharCode(e.keyCode));
            }

       }
       else if (e.keyCode==8 || e.keyCode==46){ //Cas de suppression


        if (inc!='f'){

        
           if (inc>=(longueur_mot*étape)){
            $(L[inc]).val('');
           }
           if(inc-1>=longueur_mot*étape-1){
               inc-=1;
           }
       }
    }
       else if (e.keyCode==13){ //Cas de validation
           if (inc!='f'){//Partie non finie
            if (testcase(étape)){//Vérification si le mot est vide
                var begining=get_cases(étape);
                var dico=const_mot(begining);
                if (mots.includes(dico)){ //Mot est dans le dictionnaire ?

               if (étape+1<nombre_dessais){ //Vérification si il nous reste encore un essai
                   étape+=1; 
                   var ell1=get_cases(étape-1);
                   var guess=const_mot(ell1);

                   if (!check_win(guess)){
                       var ell2=get_cases(étape);


                       var copy2=mot_à_deviner.slice();
                       var copy=guess.slice();
                       var neutre=new_sequence(guess);
                       var t=bonnes_cases(copy,copy2,neutre);
                       var change=bonnes_lettres(t[0],t[1],t[2]);
                       change_colors(ell1,change);
                       inc=longueur_mot*étape-1;
                       sleep(longueur_mot*200).then(()=>{set_cases(ell2,saisie_auto);});


                   }
                   else{
                       var copy=mot_à_deviner.slice();
                       var seq=sequence(guess,copy);
                       change_colors(ell1,seq);
                       sleep(200*longueur_mot).then(() => {
                        var audio_victoire = new Audio('/static/Sons motus/celebration_BCb02kw.mp3');
                        audio_victoire.play();
                        });
                       inc='f'; /**On met inc à 'f' pour finir la partie */
                       var pattern=[];
                       var temp_mot="";
                       var ell=[];
                       for (var k=0;k<étape;k++){
                           ell=get_cases(k);
                           temp_mot=const_mot(ell);
                           pattern.push(temp_mot);

                       }
                       $(".pattern").val(pattern);
                       sleep(200*longueur_mot+5000).then(() => document.myform.submit());

                   }
               }
               else{
                   var copy=mot_à_deviner.slice();
                   var ell1=get_cases(étape);
                   var check=const_mot(ell1);
                   if (check_win(check)){
                       /** Victoire sur le dernier try */
                       var copy=mot_à_deviner.slice();
                       var seq=sequence(check,copy);
                       change_colors(ell1,seq);
                       sleep(200*longueur_mot).then(() => {
                        var audio_victoire = new Audio('/static/Sons motus/celebration_BCb02kw.mp3');
                        audio_victoire.play();
                        });
                       inc='f';
                       var pattern=[];
                       var temp_mot="";
                       var ell=[];
                       for (var k=0;k<nombre_dessais;k++){
                           ell=get_cases(k);
                           temp_mot=const_mot(ell);
                           pattern.push(temp_mot);

                       }
                       $(".pattern").val(pattern);
                       sleep(longueur_mot*200+5000).then(() => document.myform.submit());
                   }
                   else{
                       /**Cas de défaite, nbr de trys dépassé */
                       inc='f';
                       sleep(195*longueur_mot).then(() => {
                       var audio_defaite = new Audio('/static/Sons motus/motus-boule-noire.mp3');
                       audio_defaite.play();
                       });
                       var seq=sequence(check,copy);
                       change_colors(ell1,seq);
                       var pattern=[];
                       var temp_mot="";
                       var ell=[];
                       for (var k=0;k<nombre_dessais;k++){
                           ell=get_cases(k);
                           temp_mot=const_mot(ell);
                           pattern.push(temp_mot);

                       }
                       $(".pattern").val(pattern);
                       sleep(195*longueur_mot+4000).then(() => document.myform.submit());

                   }
               }


            }
            else{
                alert("Le mot saisi n'est pas dans notre dictionnaire");
            }


           }
           else{
               alert('Tous les champs doivent être remplis !');
           }
       }
       else{
       }
    }

       })

       


var slideIndex = 1;
for (var k=0;k<longueur;k++){
    showSlides(slideIndex,k);
}


function plusSlides(n,m) {
  showSlides(slideIndex += n,m);
}

function currentSlide(n,m) {
  showSlides(slideIndex = n,m);
}

function showSlides(n,m) {
  var i;
  var slidest = document.getElementsByClassName("mySlides");
  var dotst = document.getElementsByClassName("dot");
  var slides=[];
  var dots=[];
  for (var k=0;k<slidest.length;k++){
      if (((slidest[k]).id)==(m.toString())){
          slides.push(slidest[k])
      }
  }
  for (var k=0;k<dotst.length;k++){
    if ((dotst[k]).id==m.toString()){
        dots.push(dotst[k])
    }
}

  if (n > slides.length) {slideIndex = 1}    
  if (n < 1) {slideIndex = slides.length}
  for (i = 0; i < slides.length; i++) {
      slides[i].style.display = "none";  
  }
  for (i = 0; i < dots.length; i++) {
      dots[i].className = dots[i].className.replace(" active", "");
  }
  slides[slideIndex-1].style.display = "block";  
  dots[slideIndex-1].className += " active";
}
    
