/* Copyright (C) 2022 Clément Bourdier
 * All rights reserved.
 *
 * Projet Domotique
 * Ecole du Web
 * Cours Projets Technologiques (c)2022
 *
    @file     server.js
    @author   Clément Bourdier
    @version  1.1 09/05/22
    @description
      API permettant la gestion des historiques du projet domotique

    platform = PC portable
    OS = Windows
    Langage : javascript

    Fonctionnalités implantées
        nouvel enregistrement dans l'historique
        recuperation de l'historique d'un peripherique
        verification de la validité d'une puce rfid

 * */
const express = require('express')
const app = express()
const moogoose = require('mongoose')

moogoose.connect("mongodb://localhost/projetTechno", { useNewUrlParser: true })
const tdb = moogoose.connection
tdb.on('error', (error) => console.error(error))
tdb.once('open', () => console.log("Connecté à la base de données"))

app.use(express.json())



const historiqueRoute = require ('./routes/historique')
const rfidRoute = require ('./routes/rfid')


app.use('/api/historique', historiqueRoute)
app.use('/api/rfid', rfidRoute)

app.listen(3000, () => console.log("Le serveur a démarré"))