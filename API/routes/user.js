const express = require('express');
const router = express.Router();
const user = require('../controller/user');
const auth = require('../middleware/auth');
const unUser = require('../models/user');

//Connexion
router.post('/login', user.login);

//Créer un nouveau utilisateur
router.post('/creer', async (req, res) => {
    const unuser = new unUser({
        user: req.body.user,
        password: req.body.password,
        codeRFID: req.body.codeRFID
    })
    try{
        const nouveauUser = await unuser.save()
        res.status(201).json(nouveauUser)
    }
    catch(err){
        res.status(400).json({message: err.message})
    }
})

router.get('/authRFID', checkAuthRFID, (req, res) => {
    res.send(res.reponse)
})


// Vérifie si le code rfid est correct
async function checkAuthRFID(req, res, next) {
    let reponse
    //donnees = '"peripherique.nom":"'+req.params.name+'"';
    try {
        reponse = await user.find({ "codeRFID": req.params.codeRFID })
        if (reponse == null) {
            return res.status(404).json({ message: "Impossible de trouver le code rfid" })
        }
    } catch (err) {
        return res.status(500).json({ message: err.message })
    }
    console.log(reponse)
    res.reponse = reponse
    next()
}

module.exports = router;