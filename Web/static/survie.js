function testcase(step){
    var ell=$('.case');
    for(var k=step*longueur_mot;k<(step+1)*longueur_mot;k++){
        if (ell[k].value==''){
            return false;
        }

    }
    return true;
}
function newlen(){
    var p=Math.random();
    if (p>=0 && p<0.3){
    return 6
    }
    else if (p>=0.3 && p<0.55){
    return 7
    }
    
    else if (p>=0.55 && p<0.75){
    return 8
    }
    else if (p>=0.75 && p<0.9){
    return 9
    }   
    else{
    return 10
    }
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

var étape=0;
var k=0;
var inc=-1;
var given = document.querySelector('.given');
var toguess=document.querySelector('.toguess');
toguess.value+=mot_à_deviner+',';

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

var saisie_auto=[];
    for (var k=0;k<longueur_mot;k++){
        saisie_auto.push('.');
    }
function set_cases(ell,motList){
    for (var k=0;k<motList.length;k++){
    if (motList[k]!='.'){
      $(ell[k]).val(motList[k]);  
        }
    }
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


function removeAllChildNodes(parent) {
    while (parent.firstChild) {
        parent.removeChild(parent.firstChild);
    }
}

function genere(){
    var long=newlen();
    var newmots=[];
    for (var k=0;k<mots.length;k++){
        if (mots[k].length==long){
            newmots.push(mots[k]);

        }
    }
    var choice=Math.floor(Math.random()*newmots.length);
    return newmots[choice];
}










   

$(document).keyup(function (e) 
{   var L=document.querySelectorAll('.case'); 
       if (e.keyCode >=65 && e.keyCode<=90){
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
                   given.value+=guess;


                   if (!check_win(guess)){
                       var ell2=get_cases(étape);
                       given.value+=',';
                       var copy2=mot_à_deviner.slice();
                       var copy=guess.slice();
                       /** var seq=sequence(guess,copy); */
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
                       given.value+=';';
                        change_colors(ell1,seq); /** C'est là qu'on génère un nouveau mot */
                       inc='f'; /**On met inc à 'f' pour finir la partie */
                       var mid=document.querySelector('.midsurvie');
                       sleep(3000).then(()=>{
                        
                        var childs=mid.childNodes;
                        for (var k=0;k<childs.length;k++){
                        $(childs[k]).toggle("fade");
                        }
                        });
                        sleep(4000).then(()=>{
                            removeAllChildNodes(mid);
                            sleep(100).then(()=>{
                                var noeuds=[];
                                var ajout=[];
                                for (var i=0;i<10;i++){
                                    ajout=[];
                                    for (var j=0;j<longueur_mot;j++){
                                        ajout.push(document.createElement("input"));
                                        console.log("crée");
                                        
                                        
        
                                        
                                    }
                                    noeuds.push(ajout);
                                }
                                for (var i=0;i<10;i++){
                                    for (var j=0;j<longueur_mot;j++){
                                        noeuds[i][j].type="text";
                                        noeuds[i][j].name=i+'f'+j;
                                        noeuds[i][j].autocomplete="off";
                                        noeuds[i][j].className="case";
                                        noeuds[i][j].maxLength=1;
                                        noeuds[i][j].disabled=true;
                                        noeuds[i][j].display="none";
                                        mid.appendChild(noeuds[i][j]);
                                        mid.append(" ");
                                    }
                                    mid.appendChild(document.createElement("br"));

                                }

                                inc=-1;
                                étape=0;
                                sleep(longueur_mot*200).then(()=>{saisie_auto=[];
                                    for (var k=0;k<longueur_mot;k++){
                                    saisie_auto.push('.');
                                    }});
                            });
                        });
                        sleep(1000).then(()=>{
                        mot_à_deviner=genere();
                        toguess.value+=mot_à_deviner+','
                        console.log(mot_à_deviner); /** On laisse pour l'instant pour les tests */
                        longueur_mot=mot_à_deviner.length;









                   });
               }
            }
        
               else{
                   var copy=mot_à_deviner.slice();
                   var ell1=get_cases(étape);
                   var guess=const_mot(ell1);
                   given.value+=guess; // On rajoute le mot donné
                   if (check_win(guess)){
                       /** Victoire sur le dernier try */
                       var copy=mot_à_deviner.slice();
                       var seq=sequence(guess,copy);
                        change_colors(ell1,seq); /** C'est là qu'on génère un nouveau mot */
                       inc='f'; /**On met inc à 'f' pour finir la partie */
                       var mid=document.querySelector('.midsurvie');
                       given.value+=';'; //Séparateur des grilles 
                       sleep(3000).then(()=>{
                        
                        var childs=mid.childNodes;
                        for (var k=0;k<childs.length;k++){
                        $(childs[k]).toggle("fade");
                        }
                        });
                        sleep(4000).then(()=>{
                            removeAllChildNodes(mid);
                            sleep(100).then(()=>{
                                var noeuds=[];
                                var ajout=[];
                                for (var i=0;i<10;i++){
                                    ajout=[];
                                    for (var j=0;j<longueur_mot;j++){
                                        ajout.push(document.createElement("input"));
                                        console.log("crée");
                                        
                                        
        
                                        
                                    }
                                    noeuds.push(ajout);
                                }
                                for (var i=0;i<10;i++){
                                    for (var j=0;j<longueur_mot;j++){
                                        noeuds[i][j].type="text";
                                        noeuds[i][j].name=i+'f'+j;
                                        noeuds[i][j].autocomplete="off";
                                        noeuds[i][j].className="case";
                                        noeuds[i][j].maxLength=1;
                                        noeuds[i][j].disabled=true;
                                        noeuds[i][j].display="none";
                                        mid.appendChild(noeuds[i][j]);
                                        mid.append(" ");
                                    }
                                    mid.appendChild(document.createElement("br"));

                                }

                                inc=-1;
                                étape=0;
                                sleep(longueur_mot*200).then(()=>{saisie_auto=[];
                                for (var k=0;k<longueur_mot;k++){
                                saisie_auto.push('.');
                                }});
                            });
                        });
                        sleep(1000).then(()=>{
                        mot_à_deviner=genere();
                        toguess.value+=mot_à_deviner+',';
                        console.log(mot_à_deviner); /** On laisse pour l'instant pour les tests */
                        longueur_mot=mot_à_deviner.length;
 
                        
                        })

                    }
                   else{
                       /**Cas de défaite, nbr de trys dépassé */
                       inc='p'; /** On met la valeur incrément à p pour signifier la défaite */
                       var copy=mot_à_deviner.slice();
                       var seq=sequence(guess,copy);
                       change_colors(ell1,seq);
                       console.log("Perdu"); //Penser à effectuer la redirection
                       sleep(longueur_mot*300).then(()=>{
                        $(".notime").val(TempsSurvécu);
                        clearInterval(DimChrono);
                        document.myform.submit();
                       })

                   }

               }





           }
           else{
               alert('Le mot saisi  n\'est pas dans notre dictionnaire');
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

       



    
