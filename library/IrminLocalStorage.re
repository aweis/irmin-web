/**
 *
 */
module Read_only = (K: Irmin.Type.S, V: Irmin.Type.S) => {
  type key = K.t;
  type value = V.t;

  type t('a) = {t: LocalStorage.t};

  let batch = (t, f) => f(t);

  let pp_key = Irmin.Type.pp(K.t);

  let find = ({t, _}, key) =>
    switch (LocalStorage.get(t, key)) {
    | None => ""
    | Some(s) => s
    };
  let mem = ({t, _}, key) =>
    switch (LocalStorage.get(t, key)) {
    | None => false
    | Some(_s) => true
    };
};

module Append_only = (K: Irmin.Type.S, V: Irmin.Type.S) => {
  include Read_only(K, V);

  let add = LocalStorage.set;
};
