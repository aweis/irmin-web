module Helper = (K: Irmin.Contents.S0, V: Irmin.Contents.S0) => {
  type t('a) = string;
  type key = K.t;
  type value = V.t;

  let v = () => LocalStorage.make();

  let mem = (t, key) =>
    switch (LocalStorage.get(t, key)) {
    | None => false
    | Some(_v) => true
    };

  let find = (t, key) => LocalStorage.get(t, key);
};

module ContentAddressable = (K: Irmin.Hash.S, V: Irmin.Contents.S0) => {
  include Helper(K, V);
  let add = (t, key, value) => {
    LocalStorage.set(t, key, value);
    key;
  };
  let batch = (t, f) => f(t);
};
