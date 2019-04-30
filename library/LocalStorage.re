open Js_of_ocaml;
class type storageEvent = {
  inherit Dom_html.event;
  pub key: Js.readonly_prop(Js.t(Js.js_string));
  pub oldValue: Js.readonly_prop(Js.opt(Js.t(Js.js_string)));
  pub newValue: Js.readonly_prop(Js.opt(Js.t(Js.js_string)));
  pub url: Js.readonly_prop(Js.t(Js.js_string));
  pub storageArea: Js.readonly_prop(Js.opt(Js.t(Dom_html.storage)));
};

type t = Js.t(Dom_html.storage);
type key = string;

let make = () =>
  Js.Optdef.get(Dom_html.window##.localStorage, () =>
    failwith("localStorage not available")
  );

let get = (t, key) =>
  Js.Opt.case(
    t##getItem(Js.string(key)),
    () => None,
    v => Some(Js.to_string(v)),
  );

let set = (t, key, value) => {
  let encoded = value |> Js.string;
  t##setItem(Js.string(key), encoded);
};

let remove = (t, key) => t##removeItem(Js.string(key));

let event = Dom.Event.make("storage");

let key = (ev: Js.t(#storageEvent)) => ev##.key;

let watch = (t, ~prefix, cb) => {
  let onChange = (ev: Js.t(storageEvent)) => {
    if (ev##.storageArea == Js.Opt.return(t)) {
      let k = key(ev);
      if (k##lastIndexOf_from(Js.string(prefix), 0) == 0) {
        let k = Js.to_string(k);
        let v =
          Js.Opt.case(
            ev##.newValue,
            () => None,
            v => Some(Js.to_string(v)),
          );
        cb(k, v);
      };
    };
    Js._true;
  };
  Dom.addEventListener(
    Dom_html.window,
    event,
    Dom.handler(onChange),
    Js._true,
  );
};
