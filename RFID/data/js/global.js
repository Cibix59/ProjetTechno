/*****************************************************
 * Fichier: global.js
 * Description: c'est le fichier javascript du site web
 * Date: 23/02/22
 * Auteur: Clément Bourdier
 *****************************************************
 */


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