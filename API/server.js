//require('dotenv').config()

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