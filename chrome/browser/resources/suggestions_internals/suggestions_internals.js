// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * Javascript for suggestions_internals.html, served from
 * chrome://suggestions-internals/. This is used to debug suggestions ranking.
 * When loaded, the page will show the current set of suggestions, along with a
 * large set of information (e.g. all the signals that were taken into
 * consideration for deciding which pages were selected to be shown to the user)
 * that will aid in debugging and optimizing the algorithms.
 */
cr.define('suggestionsInternals', function() {
  'use strict';

  /**
   * Register our event handlers.
   */
  function initialize() {
    $('suggestions-form').addEventListener('submit', onRefreshClicked);
    $('show-discarded').addEventListener('change', refresh);
    refresh();
  }

  /**
   * Called when the 'Refresh' button is clicked. Reloads the suggestions data.
   */
  function onRefreshClicked(event) {
    refresh();
    event.preventDefault();
  }

  /**
   * Reloads the suggestions data by sending a 'getSuggestions' message to
   * Chrome. The C++ code is then expected to call 'setSuggestions' when there
   * are suggestions ready.
   */
  function refresh() {
    chrome.send('getSuggestions');
  }

  /**
   * A list of columns that we do not want to display.
   * @type {Array.<string>}
   * @const
   */
  var IGNORED_COLUMNS = [
    'direction'
  ];

  /**
   * A list specifying the name of the first columns to be displayed. If
   * present, they will be displayed in this order, followed by the remaining
   * columns.
   * @type {Array.<string>}
   * @const
   */
  var PREFERRED_COLUMN_ORDER = [
    'title',
    'url',
    'score'
  ];

  /**
   * Called by Chrome code, with a ranked list of suggestions. The columns
   * to be displayed are calculated automatically from the properties of the
   * elements in the list, such that all properties have a column.
   */
  function setSuggestions(list) {
    // Build a list of all the columns that will be displayed.
    var columns = [];
    list.forEach(function(entry) {
      for (var column in entry) {
        if (typeof(entry[column]) == 'object') {
          // Expand one level deep
          for (var sub_column in entry[column]) {
            var path = column + '.' + sub_column;
            if (columns.indexOf(path) < 0)
              columns.push(path);
          }
        } else if (columns.indexOf(column) < 0) {
          columns.push(column);
        }
      }
    });

    // Remove columns that we don't want to display.
    columns = columns.filter(function(column) {
      return IGNORED_COLUMNS.indexOf(column) < 0;
    });

    // Move the preferred columns to the start of the column list.
    for (var i = PREFERRED_COLUMN_ORDER.length - 1; i >= 0; i--) {
      var index = columns.indexOf(PREFERRED_COLUMN_ORDER[i]);
      if (index >= 0)
        columns.unshift(columns.splice(index, 1)[0]);
    }

    // Special columns.
    columns.unshift('favicon');
    columns.unshift('screenshot');
    columns.unshift('rank');

    // Erase whatever is currently being displayed.
    var output = $('suggestions-debug-text');
    output.innerHTML = '';

    // Create the container table and add the header row.
    var table = document.createElement('table');
    table.className = 'suggestions-debug-table';
    var header = document.createElement('tr');
    columns.forEach(function(entry) {
      var column = document.createElement('th');
      column.innerText = entry;
      header.appendChild(column);
    });
    table.appendChild(header);

    // Add all the suggestions to the table.
    var rank = 1;
    list.forEach(function(entry) {
      var row = document.createElement('tr');
      columns.forEach(function(column_name) {
        var column = document.createElement('td');
        // Expand the path and find the data if it's there.
        var path = column_name.split('.');
        var data = entry;
        for (var i = 0; i < path.length; ++i) {
          if (data && data.hasOwnProperty(path[i]))
            data = data[path[i]];
          else
            data = undefined;
        }
        // Only add the column if the current suggestion has this property
        // (otherwise, leave the cell empty).
        if (data) {
          // If the text is a URL, make it an anchor element.
          if (/^https?:\/\/.+$/.test(data)) {
            var anchor = document.createElement('a');
            anchor.href = data;
            anchor.innerText = data;
            column.appendChild(anchor);
          } else {
            column.innerText = data;
          }
        } else if (column_name == 'rank') {
          column.innerText = rank++;
        } else if (column_name == 'screenshot') {
          var thumbnailUrl = 'chrome://thumb/' + entry.url;
          var img = document.createElement('img');
          img.onload = function() {
            column.innerText = 'Y';
            column.classList.add('has-screenshot');
          }
          img.onerror = function() { column.innerText = 'N'; }
          img.src = thumbnailUrl;
        } else if (column_name == 'favicon') {
          var faviconUrl = 'chrome://favicon/size/16/' + entry.url;
          column.style.backgroundImage = url(faviconUrl);
          column.style.backgroundRepeat = 'no-repeat';
          column.style.backgroundPosition = 'center center';
        }
        row.appendChild(column);
      });
      table.appendChild(row);
    });

    output.appendChild(table);
  }

  return {
    initialize: initialize,
    setSuggestions: setSuggestions
  };
});

document.addEventListener('DOMContentLoaded', suggestionsInternals.initialize);
