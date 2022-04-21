 function testcase(ell){
     for(var k=0;k<ell.length;k++){
         if (ell[k].value=='' && !(ell[k].disabled)){
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

var mot_à_deviner="PELLE";
var longueur_mot=5; /**à modifier */
var nombre_dessais=2;
var étape=0;
var k=0;

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
    var audio_bonne_lettre = new Audio('motus-lettre-bonne.mp3');
    var audio_mauvaise_lettre = new Audio('uuhhh_KD275SD.mp3');
    var audio_mal_place = new Audio('denis_ha.mp3');
    $(ell).each(function(i)
    {
    var $this = $(this);
          
    setTimeout(function() {
        if (sequence[i]=='1'){
            $this.switchClass("","y",300);
            audio_mal_place.play();

        }
        else if(sequence[i]=='2'){
            $this.switchClass("","g",300);
            audio_bonne_lettre.play();
        }
        else{
            audio_mauvaise_lettre.play();
            
        }


    }, t);
          
    t += 800;
    
    })
}




function sleep (time) {
    return new Promise((resolve) => setTimeout(resolve, time));
}



    

$(document).ready(function () 
{   var L=document.querySelectorAll('.case');
    $('.case').keyup(function (e)
    {   
        if (e.keyCode >=65 && e.keyCode<=90){
            $(this).val(String.fromCharCode(e.keyCode));
            $(this).animate({height:"53px",width:"53px"},"fast"); /** effet visuel pour l'entrée de lettres */
            $(this).animate({height:"50px",width:"50px"},"fast");            
            $(this).next().focus();
        }
        else if (e.keyCode==8 || e.keyCode==46){
            $(this).val('');

        }
        else if (e.keyCode==13){

            if (testcase(L)){
                if (étape+1<nombre_dessais){
                    
                    étape+=1;
                    var ell1=get_cases(étape-1);
                    var guess=const_mot(ell1);

                    if (!check_win(guess)){
                        var ell2=get_cases(étape);
                        enable(ell2);
                        $(ell2[0]).focus();
                        var copy2=mot_à_deviner.slice();
                        var copy=guess.slice();
                        /** var seq=sequence(guess,copy); */
                        var neutre=new_sequence(guess);
                        var t=bonnes_cases(copy,copy2,neutre);
                        var change=bonnes_lettres(t[0],t[1],t[2]);
                        change_colors(ell1,change); 
                        disable(ell1);
                        

                    }
                    else{
                        var copy=mot_à_deviner.slice();
                        var seq=sequence(guess,copy);
                        change_colors(ell1,seq);
                        disable(ell1);
                        sleep(3500).then(() => {
                            var audio_victoire = new Audio('celebration_BCb02kw.mp3');
                            audio_victoire.play();
                        });
                    }


                }
                else{
                    var ell1=get_cases(étape);
                    var check=const_mot(ell1);
                    if (check_win(check)){
                        var copy=mot_à_deviner.slice();
                        var seq=sequence(check,copy);
                        change_colors(ell1,seq);
                        disable(ell1);
                    }
                    else{
                        var audio_defaite = new Audio('motus-boule-noire.mp3');
                        var copy=mot_à_deviner.slice();
                        var seq=sequence(check,copy);
                        change_colors(ell1,seq);
                        disable(ell1);
                        sleep(3500).then(() => {            /**A MODIFIER AVEC LONGUEUR MOT */
                        audio_defaite.play();
                        console.log('Perdu');
                        });
                    }
                }




            }
            else{
                alert('Tous les champs doivent être remplis !')
            }
        }
        else if (e.which==9){
            alert('Veuillez remplir les champs les uns après les autres');
            $(this).prev().focus();
    
        }
        else {
            $(this).val('');
            alert('Veuillez ne rentrer que des lettres')
        }
        


        }) 
        ;
 
    });
    
 


