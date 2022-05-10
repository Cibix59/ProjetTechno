const express = require('express');
const router = express.Router();
const RFID = require('../models/rfid');




//Créer un nouveau rfid
router.post('/creer', async (req, res) => {

    const rfid = new RFID({
        codeRFID: req.body.codeRFID
    })
    try{
        const nouveauRFID = await rfid.save()
        res.status(201).json(nouveauRFID)
    }
    catch(err){
        res.status(400).json({message: err.message})
    }
})


router.put('/ajouterRFID', async (req, res) => {
    
    try {
        rfid = await RFID.find({ "codeRFID": req.body.codeRFID })
        if (rfid == null ) {
            return res.status(404).json({ message: "Impossible de trouver le code rfid" })
        }
    } catch (err) {
        return res.status(500).json({ message: err.message })
    }
    

    const rfid = new RFID({
        codeRFID: req.body.codeRFID
    })
    try{
        const nouveauRFID = await rfid.save()
        res.status(201).json(nouveauRFID)
    }
    catch(err){
        res.status(400).json({message: err.message})
    }
})

/* 
ajout d'elements à la liste porte
$push : {
    rfid :  {
             "porte": 1
           }
  } */

router.post('/authRFID', checkAuthRFID, (req, res) => {
    res.send(res.reponse)
})


// Vérifie si le code rfid est correct
async function checkAuthRFID(req, res, next) {

    console.log(req.body.codeRFID)
    let reponse
    try {
        reponse = await RFID.find({ "codeRFID": req.body.codeRFID })
        if (reponse == null ) {
            return res.status(404).json({ message: "Impossible de trouver le code rfid" })
        }
    } catch (err) {
        return res.status(500).json({ message: err.message })
    }

    res.reponse = reponse
    next()
}

module.exports = router;