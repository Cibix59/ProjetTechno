/*****************************************************
 * Fichier: global.js
 * Description: c'est le fichier javascript du site web
 * Date: 23/02/22
 * Auteur: Clément Bourdier
 *****************************************************
 */


function test() {
    console.log("test");
}




// fonctions qui permet de récupérer le témperature du système.
setInterval(function getInfosFromESP() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function () {
        if (this.status == 200) {
            console.debug("retour 200");
            console.debug(this.responseText);
            infos = this.responseText.split(";");

            document.getElementById("ip").innerHTML = infos[0];
            document.getElementById("port").innerHTML = infos[1];
            document.getElementById("intervalle").innerHTML = infos[2];
        }
    };
    xhttp.open("GET", "getInfosFromESP", true);
    xhttp.send();
}, 1000);


// fonctions qui permet d'envoyer une nouvelle information à l'esp
function envoisInfosToEsp(typeInfo) {
    var xhttp = new XMLHttpRequest();
    xhttp.open("POST", "envoisInfosToEsp", true);
    xhttp.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
    var params = "";
    info = document.getElementById(typeInfo).value;
    console.log(info);

    params = String(typeInfo) + String("=") + String(info);

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