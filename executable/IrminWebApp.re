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
print_endline("FOOBAR2");

module Mem_store = Irmin_mem.KV(Irmin.Contents.String);

let config = Irmin_mem.config();

let repo = Mem_store.Repo.v(config);

open Lwt.Infix;
let master = config => Mem_store.Repo.v(config) >>= Mem_store.master;

let info = msg => {
  let date = Int64.zero;
  let author = "AWEIS";
  Irmin.Info.v(~date, ~author, msg);
};

print_endline("AWEIS");
let infof = fmt => Fmt.kstrf((str, ()) => info(str), fmt);
let main =
  Mem_store.Repo.v(config)
  >>= Mem_store.master
  >>= (
    t =>
      {
        /* Set a/b/c to "Hello, Irmin!" */

        print_endline("DOING STUFF");
        Mem_store.set_exn(
          t,
          ["a", "b", "c"],
          "Hello, Irmin! in the store",
          ~info=infof("COMMIT FROM ADAM"),
        );
      }
      >>= (
        () => {
          print_endline("MORE STUFF");
          /* Get a/b/c */
          Mem_store.get(t, ["a", "b", "c"])
          >|= (s => print_endline("this is s " ++ s));
        }
      )
  );

print_endline("MADE IT 3");
