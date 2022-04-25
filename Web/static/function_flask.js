function testcase(step){
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

function get_cases(step){ /**pré-condition étape<nombre de trys max */
   var L =document.querySelectorAll(".case");
   var ell=[];
   for (var k=step*longueur_mot;k<(step+1)*longueur_mot;k++){
       ell.push(L[k]);

   }
   return ell;

}
function bonnes_cases(mot,mot2,sequ){
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

function bonnes_lettres(mot,mot2,sequence){
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

function new_sequence(mot){
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


function const_mot(ell){
   var cons='';
   for (var k=0;k<ell.length;k++){
       cons+=ell[k].value;
   }
   return cons;

   
}

function change_colors(ell,sequence){
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










   

$(document).keyup(function (e) 
{   var L=document.querySelectorAll('.case'); 
       if (e.keyCode >=65 && e.keyCode<=90){
        if (inc!='f'){  
        
            if(inc<(longueur_mot)*(étape+1)-1){
                inc+=1;
            }
            
            $(L[inc]).val(String.fromCharCode(e.keyCode));
            $(L[inc]).animate({height:"53px",width:"53px"},"fast"); /** effet visuel pour l'entrée de lettres */
            $(L[inc]).animate({height:"50px",width:"50px"},"fast");
            }

       }
       else if (e.keyCode==8 || e.keyCode==46){


        if (inc!='f'){

        
           if (inc>=(longueur_mot*étape)){
            $(L[inc]).val('');
           }
           if(inc-1>=longueur_mot*étape-1){
               inc-=1;
           }
           console.log(inc);
       }
    }
       else if (e.keyCode==13){
           if (inc!='f'){
            if (testcase(étape)){
                var begining=get_cases(étape);
                var dico=const_mot(begining);
                if (mots.includes(dico)){

               if (étape+1<nombre_dessais){
                   étape+=1;
                   var ell1=get_cases(étape-1);
                   var guess=const_mot(ell1);

                   if (!check_win(guess)){
                       var ell2=get_cases(étape);


                       var copy2=mot_à_deviner.slice();
                       var copy=guess.slice();
                       /** var seq=sequence(guess,copy); */
                       var neutre=new_sequence(guess);
                       var t=bonnes_cases(copy,copy2,neutre);
                       var change=bonnes_lettres(t[0],t[1],t[2]);
                       change_colors(ell1,change); 


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
                       for (var k=0;k<=étape;k++){
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
                       inc='f';
                       var pattern=[];
                       var temp_mot="";
                       var ell=[];
                       for (var k=0;k<étape;k++){
                           ell=get_cases(k);
                           temp_mot=const_mot(ell);
                           pattern.push(temp_mot);

                       }
                       $(".pattern").val(pattern);
                       sleep(longueur_mot*310).then(() => document.myform.submit());
                   }
                   else{
                       /**Cas de défaite, nbr de trys dépassé */
                       inc='f';
                       console.log("Perdu");
                       sleep(195*longueur_mot).then(() => {
                       var audio_defaite = new Audio('/static/Sons motus/motus-boule-noire.mp3');
                       audio_defaite.play();
                       console.log('Perdu');
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
           /**Redirection avec stockage des données */
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
  console.log(m);
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
    
