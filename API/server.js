//require('dotenv').config()

const express = require('express')
const app = express()
const moogoose = require('mongoose')

moogoose.connect("mongodb://localhost/projetTechno", { useNewUrlParser: true })
const tdb = moogoose.connection
tdb.on('error', (error) => console.error(error))
tdb.once('open', () => console.log("Connecté à la base de données"))

app.use(express.json())


const userRoute = require ('./routes/user')
const historiqueRoute = require ('./routes/historique')

app.use('/api/users', userRoute)
app.use('/api/historique', historiqueRoute)

app.listen(3000, () => console.log("Le serveur a démarré"))