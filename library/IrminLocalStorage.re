/**
 *
 */
open Lwt.Infix;
module Read_only = (K: Irmin.Type.S, V: Irmin.Type.S) => {
  type key = K.t;
  type value = V.t;

  type t('a) = {t: LocalStorage.t};

  let map = {t: LocalStorage.make()};

  let v = _config => map;

  let batch = (t, f) => f(t);

  let find = ({t, _}, key) =>
    Lwt.return(
      switch (LocalStorage.get(t, key)) {
      | None => ""
      | Some(s) => s
      },
    );
  let mem = ({t, _}, key) =>
    Lwt.return(
      switch (LocalStorage.get(t, key)) {
      | None => false
      | Some(_s) => true
      },
    );
};

module Append_only = (K: Irmin.Type.S, V: Irmin.Type.S) => {
  include Read_only(K, V);

  let add = (t, key, value) => {
    LocalStorage.set(t, key, value);
    Lwt.return();
  };
};

module Atomic_write = (K: Irmin.Type.S, V: Irmin.Type.S) => {
  module RO = Read_only(K, V);
  module W = Irmin.Private.Watch.Make(K, V);
  module L = Irmin.Private.Lock.Make(K);

  type t = {
    t: RO.t(unit),
    w: W.t,
    lock: L.t,
  };

  type key = RO.key;

  type value = RO.value;

  type watch = W.watch;

  let watches = W.v();

  let lock = L.v();

  let v = config => RO.v(config);

  let find = t => RO.find(t.t);

  let mem = t => RO.mem(t.t);

  let watch_key = t => W.watch(t.w);

  let watch = t => W.watch(t.w);

  let unwatch = t => W.unwatch(t.w);

  let list = _t => [] |> Lwt.return;

  let set = LocalStorage.set;

  let remove = LocalStorage.remove;
};

module Make =
  Irmin.Make((Irmin.Content_addressable(Append_only)), Atomic_write);

module KV = (C: Irmin.Contents.S) =>
  Make(
    Irmin.Metadata.None,
    C,
    Irmin.Path.String_list,
    Irmin.Branch.String,
    Irmin.Hash.SHA1,
  );
