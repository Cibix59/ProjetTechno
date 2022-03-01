const express = require('express');
const historique = require('../models/historique');
const router = express.Router();
//const user = require('../controller/peripheriques');
//const auth = require('../middleware/auth');
const unhistorique = require('../models/historique');

router.get('/:nom', getUnHistorique, (req, res) => {
    console.log(res.lhistorique)
    res.send(res.lhistorique)
})

//Récupère tous les noms des bois avec leurs id
router.get('/senseur', async (req, res) => {
    try {
        const senseur = await unhistorique.find(
            {},
            {
                _id: 1,
            }
        )
        res.json(senseur)
    } catch (err) {
        res.status(500).json({ message: err.message })
    }
})



//Ajout d'un nouvel enregistrement
router.post('/log', async (req, res) => {
    console.log( req.body)
    console.log( req.body.date)
    unhistorique = new historique({
        date: req.body.date,
        heure: req.body.heure,
        evennement: req.body.evennement,
        peripherique: [{ nom: req.body.nomPeripherique, description: req.body.descriptionDeripherique }]
    })

    try{
        const nouvelHistorique = await unhistorique.save()
        res.status(201).json(nouvelHistorique)
    }
    catch(err){
        res.status(400).json({message: err.message})
    }
})


/* router.get('/:nom', (req, res) => {
    let paramBois = upperFirstLetter(req.params.bois);
    Bois.findByWood(paramBois, (err, data) => {
        if (err) {
            if (err.kind === "not_found") {
                res.status(404).send({
                    message: `Le bois avec le nom ${req.params.bois} n'a pas été trouvé.`
                });
            } else {
                res.status(500).send({
                    message: "Erreur lors de l'obtention du bois ayant le nom " + req.params.bois
                });
            }
        } else res.send(data);
    });
}) */




async function getUnHistorique(req, res, next) {
    let lhistorique
    //donnees = '"peripherique.nom":"'+req.params.name+'"';
    try {
        lhistorique = await unhistorique.find({ "peripherique.nom":req.params.nom })
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