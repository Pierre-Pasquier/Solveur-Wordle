let chrono = document.getElementById("chrono");
let minutes =5
let secondes =1
let TempsSurvécu = -1
var temp_min=0;
var temp_sec=0;
var historique=new Array();
historique[0]=[1,30];
historique[1]=[1,45];
historique[2]=[2,0];
historique[3]=[2,30];
historique[4]=[3,0];
var bonus_essais=new Array();
bonus_essais[0]=[1,0]; /** (minute,secondes) */
for (var k=1;k<10;k++){
    bonus_essais[k]=[0,(10-k)*6];
}
var histoire = document.getElementById("background");
let a = 0;
let b=0;
let c=0;

var DimChrono =setInterval(function(){

    TempsSurvécu ++ ; 
    minutes = parseInt(minutes);
    secondes = parseInt(secondes);


    if(secondes == 0 && minutes >0){
        minutes--;
        secondes = 60;
    }
    secondes = secondes -1
    if(secondes == 1 && minutes == 0){
        $(".notime").val(TempsSurvécu);
        document.myform.submit();
        clearInterval(DimChrono);
    }
    if(minutes<=2 && minutes>=1){
        chrono.className = 'circleO';
    }
    if(minutes<1 ){
        chrono.className = 'circleR';
    }
    if(minutes>2){
        chrono.className= 'circleG';
    }



    //ici on mettra les condition pour augmenter le temps ou le diminuer:
    if (inc=='f' && ((secondes>=0 && minutes >0)||(secondes>0 && minutes==0))){
        if (b==0){  /** On regarde pour effectuer le changement au bon moment avec la bonne valeur de la longueur du mot */
            temp_min=historique[longueur_mot-6][0]+bonus_essais[étape-1][0];
            temp_sec=historique[longueur_mot-6][1]+bonus_essais[étape-1][1];
            temp_min+=Math.floor(temp_sec/60);
            temp_sec=temp_sec%60;
            b=1;
            secondes+=temp_sec;
            minutes+=temp_min;
            histoire.innerHTML="+ " + temp_min+" min "+temp_sec+" s";
        }

    }
    if (b==1){
        c+=1;
    }
    if (c>longueur_mot*0.5){
        b=0;
        c=0;
        histoire.innerHTML="";
    }



//    if(secondes==35){
//        secondes-=40;
//    }

    if(secondes<0){
        minutes --;
        secondes = 60-secondes;
    }

    //ici on gere les eventuels problémes liés aux variations du chrono
    if(secondes>=60){
        minutes += Math.floor(secondes/60);
        secondes = secondes-Math.floor(secondes/60)*60;
    }


    if(secondes<10){
        secondes = "0" + secondes;
    }
    if(minutes<10){
        minutes = "0" + minutes;
    }
    chrono.textContent = `${minutes}:${secondes}`;


},1000)

