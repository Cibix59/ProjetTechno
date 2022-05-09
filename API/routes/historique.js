const express = require('express');
const router = express.Router();
//const user = require('../controller/peripheriques');
//const auth = require('../middleware/auth');
const Historique = require('../models/historique');

router.get('/:nom', getUnHistorique, (req, res) => {
    console.log(res.lhistorique)
    res.send(res.lhistorique)
})



//Ajout d'un nouvel enregistrement
router.post('/log', async (req, res) => {
    console.log("crée un historique")
    unhistorique = new Historique({
        date: req.body.date,
        heure: req.body.heure,
        topic: req.body.topic,
        nomPeripherique: req.body.nomPeripherique,
        payload: req.body.payload
    })
    try {
        const nouvelHistorique = await unhistorique.save()
        console.log("pendant save historique")
        res.status(201).json(nouvelHistorique)
    }
    catch (err) {
        res.status(400).json({ message: err.message })
    }
})

//Donne l'historique d'un peripherique
async function getUnHistorique(req, res, next) {
    console.log("get un historique")
    let lhistorique
    //donnees = '"peripherique.nom":"'+req.params.name+'"';
    try {
        lhistorique = await Historique.find({ "nomPeripherique": req.params.nom })
        console.log("dedans 1")
        if (lhistorique == null) {
            return res.status(404).json({ message: "Impossible de trouver le peripherique" })
        }
    } catch (err) {
        console.log("catch")
        return res.status(500).json({ message: err.message })
    }
    console.log("fini 1")
    console.log(lhistorique)
    res.lhistorique = lhistorique
    next()
}


module.exports = router;