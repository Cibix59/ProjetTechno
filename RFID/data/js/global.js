/*****************************************************
 * Fichier: global.js
 * Description: c'est le fichier javascript du site web
 * Date: 23/02/22
 * Auteur: Clément Bourdier
 *****************************************************
 */


function test(){
    console.log("test");
}

function connexion() {
    console.log("test");
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "textEnvoyee", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

    xhttp.onreadystatechange = function () {

        if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
            if(this.responseText=="ok"){
                window.location.href = "/home";
            }
  /*            var arrayOfStrings = JSON.parse(this.responseText);
            reponse= arrayOfStrings;
            for (i = 0; i < arrayOfStrings.length; i++) {
                var x = document.getElementById("selectBois");
                var option = document.createElement("option");
                option.value = arrayOfStrings[i]["id"];
                option.text = arrayOfStrings[i]["bois"];
                x.add(option);
                }

            //Refresh le contenu
            var siteHeader = document.getElementById('selectBois');
            siteHeader.style.display='none';
            siteHeader.offsetHeight; 
            siteHeader.style.display='block';
            afficherCaracteristique(1); */ 
            }
    };


    xhttp.open("POST", "connexion", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

    login=document.getElementById("login").value;
    password=document.getElementById("password").value;
    console.log(login);
    var params = String("login") + String("=") + String(login)+String("&")+String("password") + String("=") + String(password);
    xhttp.send(params);
}


// fonctions qui permet de récupérer tous les bois via l'API.
/* function getFromESP_getAllWoodOptions() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            var arrayOfStrings = JSON.parse(this.responseText);
            listeBois= arrayOfStrings;
            for (i = 0; i < arrayOfStrings.length; i++) {
                var x = document.getElementById("selectBois");
                var option = document.createElement("option");
                option.value = arrayOfStrings[i]["id"];
                option.text = arrayOfStrings[i]["bois"];
                x.add(option);
                }

            //Refresh le contenu
            var siteHeader = document.getElementById('selectBois');
            siteHeader.style.display='none';
            siteHeader.offsetHeight; 
            siteHeader.style.display='block';
            afficherCaracteristique(1);

            }
    };
    xhttp.open("GET", "getAllWoodOptions", true);
    xhttp.send();
}

// fonctions qui permet de récupérer le témperature du système.
setInterval(function getTempsSysteme(){
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
    if(this.status == 200) {
        console.debug("retour 200");
        console.debug(this.responseText);
        
    document.getElementById("temp").innerHTML = this.responseText;
    }
    };
    xhttp.open("GET", "lireTemp", true);
    xhttp.send();
   }, 1000);

   // fonctions qui permet de déclencher le four dans la vue.
   function declencheFour() {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "declencheFour", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');

    temperature=document.getElementById("temperature").innerHTML;
    duree=document.getElementById("temps").innerHTML;

    var params = String("temperature") + String("=") + String(temperature)+String("&")+String("duree") + String("=") + String(duree);
    xhttp.send(params);
}




// fonctions intervalle qui permet d'allumer les voyants  de la vue en fonction de l'esp.
setInterval(function getStatus() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.status == 200) {
            console.debug("retour 200");
            console.debug(this.responseText);
            document.getElementById("ledRouge").style.color = "black";
            document.getElementById('ledBleu').style.color = 'black';
            document.getElementById('ledVert').style.color = 'black';
            switch (this.responseText) {
                case "off":
                    document.getElementById('ledVert').style.color = 'green';
                    break;
                case "cold":
                    document.getElementById('ledBleu').style.color = 'blue';
                    break;
                case "heat":
                    document.getElementById('ledRouge').style.color = 'red';
                    break;
                default:

            }
        }
    };
    xhttp.open("GET", "lireStatus", true);
    xhttp.send();
}, 1000); */