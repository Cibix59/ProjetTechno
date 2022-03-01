const express = require('express')
const router = express.Router()
const UnSAC = require('../models/unSAC');


router.get('/compter', async(req, res) => {
    try{
        const NombreType = await UnSAC.count();
        res.json(NombreType);
    } catch(err){
        res.status(500).json({ message: err.message })
    }
})

//Récupère tous les noms des bois avec leurs id
router.get('/nom', async(req, res) => {
    try{
        const SACNom = await UnSAC.find(
            { },
            {
                _id: 1,
                bois: 1
            }
        )
        res.json(SACNom)
    } catch(err){
        res.status(500).json({ message: err.message })
    }
})

//Obtenir toutes les informations de tous les bois
router.get('/', async (req, res) => {
    try{
        const SAC = await UnSAC.find()
        res.json(SAC)
    } catch (err){
        res.status(500).json({message: err.message})
    }
})

//Obtenir une seule information d'un bois à partir de l'id
router.get('/:id', getUnSAC, (req, res) => {
    res.send(res.unSAC)
})

//Créer un nouveau bois
router.post('/', async (req, res) => {
    const unSAC = new UnSAC({
        bois: req.body.bois,
        type: req.body.type,
        origine: req.body.origine,
        TempsDeSechage: req.body.TempsDeSechage,
        TemperatureMin: req.body.TemperatureMin
    })

    try{
        const nouveauSAC = await unSAC.save()
        res.status(201).json(nouveauSAC)
    } catch(err){
        res.status(400).json({message: err.message})
    }

})

//Mettre à jour une ou plusieurs information(s) d'un bois (l'id du bois en paramètre)
router.patch('/:id', getUnSAC, async (req, res) => {
    if(req.body.bois != null){
        res.unSAC.bois = req.body.bois
    }
    if(req.body.type != null){
        res.unSAC.type = req.body.type
    }
    if(req.body.origine != null){
        res.unSAC.origine = req.body.origine
    }
    if(req.body.TempsDeSechage != null){
        res.unSAC.TempsDeSechage = req.body.TempsDeSechage
    }
    if(req.body.TemperatureMin != null){
        res.unSAC.TemperatureMin = req.body.TemperatureMin
    }

    try{
        const MAJBois = await res.unSAC.save()
        res.json(MAJBois)
    }catch{
        res.status(400).json({ message: err.message })
    }
})

//Supprimer une information à partir de l'id du bois
router.delete('/:id', getUnSAC, async(req, res) => {
    try{
        await res.unSAC.remove()
        res.json({ message: "Le bois a été supprimé"})
    } catch(err){
        res.status(500).json({ message: err.message })
    }
})

//Retourne l'id d'un bois, sinon retourne une erreur
async function getUnSAC(req, res, next){
    let unSAC
    try{
        unSAC = await UnSAC.findById(req.params.id)
        if(unSAC == null){
            return res.status(404).json({ message: "Impossible de trouver le bois"})
        }
    } catch(err){
        return res.status(500).json({ message: err.message })
    }
    res.unSAC = unSAC
    next()
}
module.exports = router