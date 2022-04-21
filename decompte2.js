let chrono = document.getElementById("chrono");
let minutes =5
let secondes =57
let TempsSurvécu = -1
var historique=new Array();
historique[0]="+15";
historique[1]="-15";
var histoire = document.getElementById("background");
let a = 0;

const DimChrono = () =>{

    TempsSurvécu ++ ; 
    minutes = parseInt(minutes);
    secondes = parseInt(secondes);


    if(secondes == 0 && minutes >0){
        minutes--;
        secondes = 60;
    }
    if(secondes == 0 && minutes == 0){
        a=Math.floor(TempsSurvécu/60);
        b= TempsSurvécu-Math.floor(TempsSurvécu/60)*60;
        alert("Vous avez survécu : "+ a +" minute(s) " + b + " seconde(s)");
        clearTimeout(DimChrono)
    }
    if(minutes<=2 && minutes>=1){
        chrono.className = 'circleO';
    }
    if(minutes<1 ){
        chrono.className = 'circleR';
    }

    secondes = secondes -1

    //ici on mettra les condition pour augmenter le temps ou le diminuer:
    if(secondes==55){
        secondes+=75;
        histoire.className = 'historiqueV';
        histoire.innerHTML = historique[0];
    }
 
    if(histoire.className == 'historiqueV' || histoire.className == 'historiqueR'){
        a=a+1;
        if(a == 3){
            histoire.innerHTML = "";
            a=0;
        }
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

    setTimeout(DimChrono, 1000);
} 
DimChrono()
