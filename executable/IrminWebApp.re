open IrminWeb;
print_endline("Hello! irmin web app");

let x = LocalStorage.make();
LocalStorage.set(x, "adam", "weis");
let valOpt =
  switch (LocalStorage.get(x, "adam")) {
  | None => ""
  | Some(s) => s
  };
print_endline(valOpt);
let valOpt =
  switch (LocalStorage.get(x, "aweis")) {
  | None => ""
  | Some(s) => s
  };
print_endline(valOpt);

LocalStorage.remove(x, "adam");
let valOpt =
  switch (LocalStorage.get(x, "adam")) {
  | None => ""
  | Some(s) => s
  };
print_endline(valOpt);

let foo =
  LocalStorage.watch(x, ~prefix="adam", (k, v) =>
    switch (v) {
    | None => print_endline("no value for key = " ++ k)
    | Some(s) => print_endline("key = " ++ k ++ " value = " ++ s)
    }
  );

print_endline("Trying to implement the html storage store");

module Make: Irmin.S_MAKER = Irmin.Make(Irmin.AO, Irmin.RW);

module KV: Irmin.KV_MAKER =
  (C: Irmin.Contents.S) =>
    Make(
      Irmin.Metadata.None,
      C,
      Irmin.Path.String_list,
      Irmin.Branch.String,
      Irmin.Hash.SHA1,
    );
