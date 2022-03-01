const mongoose = require('mongoose')

const peripheriquesSchema = new mongoose.Schema({
    nom:{
        type: String,
        required: true
    },
    description:{
        type: String,
        required: true
    }
})

module.exports = mongoose.model("peripheriques", peripheriquesSchema);