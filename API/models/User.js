const mongoose = require('mongoose')

const userSchema = new mongoose.Schema({
    user:{
        type: String,
        required: true
    },
    password:{
        type: String,
        required: true
    },
    codeRFID:{
        type: String,
        required: false
    }
})

module.exports = mongoose.model("user", userSchema);