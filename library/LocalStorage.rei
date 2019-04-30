/**
  Local storage interface
 */
type t;
type key = string;

let make: unit => t;
let get: (t, key) => option(string);
let set: (t, key, string) => unit;
let remove: (t, key) => unit;

let watch:
  (t, ~prefix: string, (key, option(string)) => unit) =>
  Js_of_ocaml.Dom.event_listener_id;
