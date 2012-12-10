//indexController.js
module.exports = function() {
  var config = require('../config.js');
  var graph     = require('fbgraph');
  // controller 
  var self = { 
    // general requests
    indexAction : function (req, res) {
      res.render('index', { config: config});
    },
    fbLikes: function(req, res) {
      graph.get("150179641685512?fields=likes", function(err, response) {
        // output likes in this format "<NUMBER>"
        likes = '<' + response.likes + '>';
        res.end(likes);
      });
     
    }
  };
  return self;
}

