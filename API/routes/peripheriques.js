const express = require('express');
const router = express.Router();
//const user = require('../controller/peripheriques');
//const auth = require('../middleware/auth');
const unperipherique = require('../models/peripheriques');

//Connexion
//router.post('/login', user.login);


router.get('/senseur', async(req, res) => {
    try{
        const senseur = await unperipherique.find(
            { },
            {
                _id: 1,
            }
        )
        res.json(senseur)
    } catch(err){
        res.status(500).json({ message: err.message })
    }
})


//Créer un nouveau peripherique
router.post('/creer', async (req, res) => {
    console.log( req.body)
    unperipherique = new unperipherique({
        nom: req.body.nom,
        description: req.body.description
    })

    try{
        const nouveauUser = await unperipherique.save()
        res.status(201).json(nouveauUser)
    }
    catch(err){
        res.status(400).json({message: err.message})
    }
})

module.exports = router;