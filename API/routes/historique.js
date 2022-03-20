const express = require('express');
const router = express.Router();
//const user = require('../controller/peripheriques');
//const auth = require('../middleware/auth');
const Historique = require('../models/historique');

router.get('/:nom', getUnHistorique, (req, res) => {
    console.log(res.lhistorique)
    res.send(res.lhistorique)
})

router.get('/:nom', getUnHistorique, (req, res) => {
    console.log(res.lhistorique)
    res.send(res.lhistorique)
})


//Ajout d'un nouvel enregistrement
router.post('/log', async (req, res) => {

    console.log(req.body.date)
    console.log(req.body.heure)
    console.log(req.body.evennement)
    console.log(req.body.nomPeripherique)
    console.log(req.body.descriptionPeripherique)
    unhistorique = new Historique({
        date: req.body.date,
        heure: req.body.heure,
        evennement: req.body.evennement,
        nomPeripherique: req.body.nomPeripherique,
        descriptionPeripherique: req.body.descriptionPeripherique
    })

    try {
        const nouvelHistorique = await unhistorique.save()
        res.status(201).json(nouvelHistorique)
    }
    catch (err) {
        res.status(400).json({ message: err.message })
    }
})

//Donne l'historique d'un peripherique
async function getUnHistorique(req, res, next) {
    let lhistorique
    //donnees = '"peripherique.nom":"'+req.params.name+'"';
    try {
        lhistorique = await Historique.find({ "nomPeripherique": req.params.nom })
        if (lhistorique == null) {
            return res.status(404).json({ message: "Impossible de trouver le peripherique" })
        }
    } catch (err) {
        return res.status(500).json({ message: err.message })
    }
    console.log(lhistorique)
    res.lhistorique = lhistorique
    next()
}


module.exports = router;