const express = require('express');
const router = express.Router();
const unUser = require('../models/user');


//Connexion
/* router.post('/login', user.login); */

//Créer un nouveau utilisateur
router.post('/creer', async (req, res) => {
    const unuser = new unUser({
        user: req.body.user,
        password: req.body.password
    })
    try{
        const nouveauUser = await unuser.save()
        res.status(201).json(nouveauUser)
    }
    catch(err){
        res.status(400).json({message: err.message})
    }
})



module.exports = router;