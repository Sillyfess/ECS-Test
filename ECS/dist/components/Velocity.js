"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.Velocity = void 0;
const Component_1 = require("../Component");
class Velocity extends Component_1.Component {
    constructor(dx = 0, dy = 0, dz = 0) {
        super();
        this.dx = dx;
        this.dy = dy;
        this.dz = dz;
    }
    clone() {
        return new Velocity(this.dx, this.dy, this.dz);
    }
}
exports.Velocity = Velocity;
//# sourceMappingURL=Velocity.js.map