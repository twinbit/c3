//indexController.js
module.exports = function() {
  var config = require('../config.js');
  
  // controller 
  var self = { 
    // general requests
    indexAction : function (req, res) {
      res.render('index', { config: config});
    },
  };
  return self;
}

