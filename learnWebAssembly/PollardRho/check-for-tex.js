(function () {
  var body = document.body.textContent;
  if (body.match(/(?:\$|\\\(|\\\[|\\begin\{.*?})/)) {
    if (!window.MathJax) {
      window.MathJax = {
        tex: {
          inlineMath: {'[+]': [['$', '$']]}
        }
      };
    }
    var script = document.createElement('script');
    script.src = 'https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml.js';
    document.head.appendChild(script);
  }
})();
